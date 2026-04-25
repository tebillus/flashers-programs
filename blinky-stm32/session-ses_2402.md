# STM32F411 flashing trouble with dfu-util bootloader connection

**Session ID:** ses_2402e2d9bffeE7K9H4fJVkOhXm
**Created:** 4/24/2026, 10:08:12 AM
**Updated:** 4/24/2026, 10:31:30 AM

---

## User

the flashing the blinky program onto the stm32f411 is not working;

[tebillus@arch-acer blinky-stm32]$ ls
blinky.c         build02   linker_script.ld     stm32f411xx.h
blinky_memreg.c  build.sh  session-ses_2407.md  system_stm32f4xx.c
build01          flash.sh  startup.c            upload.sh
[tebillus@arch-acer blinky-stm32]$ ./flash.sh blinky
=== Building blinky ===
  CC    startup.c
  CC    system_stm32f4xx.c
  CC    blinky.c
  LD    blinky.elf
  BIN   blinky.bin
  SIZE
   text    data     bss     dec     hex filename
   1176       0       0    1176     498 build/blinky.elf

=== Build complete ===
  Binary: blinky.bin
  ELF:    build/blinky.elf

=== Flash via dfu-util ===

Binary:  blinky.bin
Address: 0x08000000

No DFU device found.
Make sure the board is in DFU mode:
  1. Hold BOOT0 (or bridge BOOT0 pin to 3.3V)
  2. Press and release RESET
  3. Release BOOT0
[tebillus@arch-acer blinky-stm32]$ ./flash.sh upload blinky.bin
=== Flash via dfu-util ===

Binary:  blinky.bin
Address: 0x08000000

No DFU device found.
Make sure the board is in DFU mode:
  1. Hold BOOT0 (or bridge BOOT0 pin to 3.3V)
  2. Press and release RESET
  3. Release BOOT0
[tebillus@arch-acer blinky-stm32]$ ./flash.sh upload blinky.bin
=== Flash via dfu-util ===

Binary:  blinky.bin
Address: 0x08000000

No DFU device found.
Make sure the board is in DFU mode:
  1. Hold BOOT0 (or bridge BOOT0 pin to 3.3V)
  2. Press and release RESET
  3. Release BOOT0
[tebillus@arch-acer blinky-stm32]$

all i see is this text from the notifications
"WeAct Studio WeAct Studio HID Bootloader has been connected." and "disconnected" when unplugged and so on
during later attempts there i was holding the BOOT0 button as i was flashing the binary too

what do 

---

## Assistant (Plan · Kimi K2.6 · 3.3s)

I'll examine the project files to understand your current flashing setup and the WeAct Studio HID bootloader situation.

**Tool: read**

**Tool: read**

**Tool: bash**

---

## Assistant (Plan · Kimi K2.6 · 4.5s)

**Tool: bash**

**Tool: websearch**

---

## Assistant (Plan · Kimi K2.6 · 3.9s)

**Tool: bash**

**Tool: read**

**Tool: read**

---

## Assistant (Plan · Kimi K2.6 · 11.3s)

Now I have a complete picture of what's happening. Let me also check the exact `hid-flash` tool syntax from the WeAct repo.

**Tool: webfetch**

---

## Assistant (Plan · Kimi K2.6 · 2.8s)

**Tool: webfetch**

---

## Assistant (Plan · Kimi K2.6 · 8.2s)

**Tool: websearch**

---

## Assistant (Plan · Kimi K2.6 · 4.7s)

**Tool: webfetch**

---

## Assistant (Plan · Kimi K2.6 · 10.6s)

I found the root cause of your flashing problem.

---

## Diagnosis

Your board has the **WeAct Studio HID Bootloader** already installed. This is **not** the STM32 built-in DFU bootloader — it is a custom 16 KB bootloader that sits at the start of flash. That is why:

1. `dfu-util` cannot find a DFU device — your board presents as a **USB HID device**, not DFU.
2. You see the notification *"WeAct Studio WeAct Studio HID Bootloader has been connected."*
3. Holding **BOOT0** does not help — BOOT0 enters the STM32 *ROM* bootloader, which bypasses the WeAct HID bootloader entirely. To enter the WeAct bootloader, you must hold the small **KEY** button (near the SWD pins), press **RESET**, then release **KEY** (the C13 LED will blink).

## Current project bugs

| File | Problem |
|------|---------|
| `linker_script.ld` | Starts at `0x08000000` — this would overwrite the bootloader. Must start at `0x08004000`. |
| `flash.sh` / `upload.sh` | Uses `dfu-util` — must use `WeAct_HID_Flash-CLI`. |

---

## Proposed Plan

1. **Download the correct flashing tool**  
   Fetch `WeAct_HID_Flash-CLI` from the [WeActStudio.MiniSTM32F4x1](https://github.com/WeActStudio/WeActStudio.MiniSTM32F4x1) repo (`Soft/WeAct_HID_FW_Bootloader/WeAct_HID_Flash-CLI`).

2. **Fix the linker script**  
   Change `ORIGIN = 0x08000000` → `0x08004000` and reduce flash length from `512K` to `496K` (512 K − 16 K bootloader).

3. **Update `flash.sh` and `upload.sh`**  
   Replace all `dfu-util` logic with `WeAct_HID_Flash-CLI <binary.bin>` and update the help text to explain the correct button sequence (hold **KEY**, press **RESET**, release **KEY**).

4. **Add udev rule** (optional but recommended)  
   Allow non-root access to the HID bootloader device.

5. **Rebuild and flash**  
   Rebuild `blinky.bin` with the new address and flash it.

---

## Questions before I proceed

1. **Does your board have the small KEY button** (usually near the 4-pin SWD header), separate from any BOOT0 jumper or button?  
   *(If you are unsure, just confirm the exact board model — e.g., WeAct Black Pill STM32F411CEU6.)*

2. **Are you okay with downloading the proprietary `WeAct_HID_Flash-CLI` binary** from GitHub, or would you prefer I also set up a fallback using the open-source `hid-flash` tool?

Once you confirm, I can implement all the fixes.

---

## User

well like, can i just wipe that weact shit clean?

---

## Assistant (Plan · Kimi K2.6 · 13.8s)

Got it. If you want to **wipe the WeAct HID bootloader completely**, you can. There are two main ways to do it. I need to know what hardware you have available so I can recommend the right path.

---

## Option 1: Overwrite it via USB DFU (no extra hardware needed)

The STM32F411 has a **built-in USB DFU bootloader in ROM** (system memory). If you force the chip into that mode, you can use `dfu-util` to flash your binary to `0x08000000`, which will **completely overwrite and erase** the WeAct bootloader.

**The catch:** Every time you want to re-flash, you have to manually enter DFU mode:
1. Hold BOOT0 (or bridge it to 3.3V)
2. Press RESET
3. Release BOOT0
4. Run `dfu-util`
5. The board resets back to your app

This is actually what your current `flash.sh` already tries to do — it just failed because the WeAct bootloader was running instead of the ROM bootloader.

---

## Option 2: Use an ST-Link / SWD debugger (if you have one)

If you have an ST-Link, J-Link, or any SWD debugger, you can just attach it and flash your binary directly to `0x08000000`. This also overwrites the WeAct bootloader. No button gymnastics needed for flashing.

---

## Questions

1. **Do you have an ST-Link (or any SWD debugger) on hand?**  
   (e.g., a cheap ST-Link V2 clone, a Black Magic Probe, or another dev board with built-in debugger?)

2. **Are you okay with pressing BOOT0 + RESET every time you want to flash new code?**  
   (That is the "pure" STM32 DFU way after wiping the WeAct loader.)

3. **Do you want to keep the ability to flash over USB without touching BOOT0?**  
   (If yes, there are cleaner open-source bootloaders like [TinyUSB's DFU](https://github.com/hathach/tinyusb) or [dapboot](https://github.com/devanlai/dapboot) that could replace WeAct's proprietary one.)

Once you tell me which route you prefer, I can write the exact steps and update your scripts accordingly.

---

## User

[tebillus@arch-acer blinky-stm32]$ ./flash.sh upload blinky.bin
=== Flash via dfu-util ===

Binary:  blinky.bin
Address: 0x08000000

DFU device detected, flashing...
dfu-util 0.11

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2021 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

dfu-util: Warning: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release
Opening DFU capable USB device...
Device ID 0483:df11
Device DFU version 011a
Claiming USB DFU Interface...
Setting Alternate Interface #0 ...
Determining device status...
DFU state(10) = dfuERROR, status(10) = Device's firmware is corrupt. It cannot return to run-time (non-DFU) operations
Clearing status
Determining device status...
DFU state(2) = dfuIDLE, status(0) = No error condition is present
DFU mode device DFU version 011a
Device returned transfer size 2048
DfuSe interface name: "Internal Flash  "
Downloading element to address = 0x08000000, size = 1176
Erase           [=========================] 100%         1176 bytes
Erase    done.
Download        [=========================] 100%         1176 bytes
Download done.
File downloaded successfully
Submitting leave request...
Transitioning to dfuMANIFEST state

Flash complete. Board will reset and run your firmware.
[tebillus@arch-acer blinky-stm32]$ 

okay, that worked; it's still plugged  in and not doing anything. what should i do now

---

## Assistant (Plan · Kimi K2.6 · 17.2s)

The DFU flash succeeded, but the board isn't blinking. That usually means the firmware either **crashed before reaching `main()`** or **never started** because the board re-entered DFU mode after reset.

Since we're in read-only plan mode, let me inspect the startup and system code to find the likely cause, then tell you what to check.

**Tool: read**

**Tool: read**

**Tool: bash**

---

## Assistant (Plan · Kimi K2.6 · 30.3s)

**Tool: websearch**

---

## Assistant (Plan · Kimi K2.6 · 23.7s)

Found it. Your board has a **25 MHz HSE crystal**, but `system_stm32f4xx.c` is configured for an **8 MHz** one.

With your current PLL settings (`PLLM=4, PLLN=96`), the VCO tries to run at 600 MHz — way above the STM32F411's 432 MHz VCO limit. The PLL never locks, so `SystemInit()` hangs in the `while (!PLLRDY)` loop forever. The LED never gets initialized.

Also, flash latency is set to 2 wait states, but 96 MHz needs 3.

---

## Plan to fix

1. **Fix `system_stm32f4xx.c`**  
   - Change PLL config for 25 MHz HSE → 96 MHz SYSCLK (e.g., `PLLM=25, PLLN=192, PLLP=2`, keeping `PLLQ=4` for a clean 48 MHz USB clock).  
   - Change flash latency from `0x2U` (2 WS) to `0x3U` (3 WS).

2. **Rebuild** with `./flash.sh blinky` (or `./flash.sh build blinky`).

3. **Re-enter DFU mode**: hold BOOT0, press RESET, release BOOT0.

4. **Flash** the new binary: `./flash.sh upload blinky.bin`.

5. **Release BOOT0** (or just reset the board) and the LED should blink.

---

## Alternative quick test

If you want to verify the diagnosis *right now* without editing code, you could enter DFU mode again and flash a binary that skips HSE entirely (uses the internal 16 MHz HSI). But the proper fix is adjusting the PLL for 25 MHz.

---

**Shall I go ahead and edit `system_stm32f4xx.c` to fix the PLL and flash latency?**  
(Or if you prefer, I can switch the clock setup to use HSI instead so it works even without the crystal.)

---

## User

1. Fix system_stm32f4xx.c  
   - Change PLL config for 25 MHz HSE → 96 MHz SYSCLK (e.g., PLLM=25, PLLN=192, PLLP=2, keeping PLLQ=4 for a clean 48 MHz USB clock).  
   - Change flash latency from 0x2U (2 WS) to 0x3U (3 WS). 

yea lets do this
provide the steps after again once done

---

## Assistant (Build · Kimi K2.6 · 7.2s)

**Tool: edit**

**Tool: edit**

---

## Assistant (Build · Kimi K2.6 · 1.8s)

**Tool: read**

**Tool: bash**

---

## Assistant (Build · Kimi K2.6 · 4.0s)

Done. `system_stm32f4xx.c` is fixed and the project rebuilt successfully.

---

## Next steps to flash and test

### 1. Enter DFU mode on the board
1. **Hold BOOT0** (or bridge the BOOT0 pin to 3.3V)
2. **Press and release RESET**
3. **Release BOOT0**

### 2. Flash the binary
```bash
./flash.sh upload blinky.bin
```

Or manually:
```bash
dfu-util -a 0 -s 0x08000000:leave -D blinky.bin
```

### 3. After flashing
The board will reset automatically and exit DFU mode. The **PC13 LED should start blinking** immediately.

---

**If it still doesn't blink**, try pressing the **RESET button once** after unplugging/replugging USB (without holding BOOT0). The ROM bootloader might have left the USB peripheral in a weird state, and a clean reset helps.

---

## User

[tebillus@arch-acer blinky-stm32]$ ./flash.sh blinky
=== Building blinky ===
  CC    startup.c
  CC    system_stm32f4xx.c
  CC    blinky.c
  LD    blinky.elf
  BIN   blinky.bin
  SIZE
   text    data     bss     dec     hex filename
   1176       0       0    1176     498 build/blinky.elf

=== Build complete ===
  Binary: blinky.bin
  ELF:    build/blinky.elf

=== Flash via dfu-util ===

Binary:  blinky.bin
Address: 0x08000000

DFU device detected, flashing...
dfu-util 0.11

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2021 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

dfu-util: Warning: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release
Opening DFU capable USB device...
Device ID 0483:df11
Device DFU version 011a
Claiming USB DFU Interface...
Setting Alternate Interface #0 ...
Determining device status...
DFU state(10) = dfuERROR, status(10) = Device's firmware is corrupt. It cannot return to run-time (non-DFU) operations
Clearing status
Determining device status...
DFU state(2) = dfuIDLE, status(0) = No error condition is present
DFU mode device DFU version 011a
Device returned transfer size 2048
DfuSe interface name: "Internal Flash  "
Downloading element to address = 0x08000000, size = 1176
Erase           [=========================] 100%         1176 bytes
Erase    done.
Download        [=========================] 100%         1176 bytes
Download done.
File downloaded successfully
Submitting leave request...
Transitioning to dfuMANIFEST state

Flash complete. Board will reset and run your firmware.
[tebillus@arch-acer blinky-stm32]$ 

sigh

---

## Assistant (Plan · Kimi K2.6 · 33.0s)

**Tool: bash**

---

## Assistant (Plan · Kimi K2.6 · 15.0s)

**Tool: grep**

**Tool: bash**

---

## Assistant (Plan · Kimi K2.6 · 8.6s)

**Tool: bash**

---

## Assistant (Plan · Kimi K2.6 · 1.7s)

**Tool: bash**

---

## Assistant (Plan · Kimi K2.6 · 33.4s)

I found the likely problem. The `main()` code is correct, but `SystemInit()` is hanging before it ever reaches `main()`. There are two strong possibilities:

## Hypothesis A: BOOT0 is still held → board re-enters DFU mode after flash

`dfu-util` leaves DFU mode by resetting the chip. **If BOOT0 is still high when the reset happens, the ROM bootloader starts again** and your firmware never runs.

**Please try this RIGHT NOW:**
1. **Make sure BOOT0 is completely released** (not touching 3.3V).
2. **Press the RESET button once** (or unplug and replug USB).
3. Check if the LED blinks.

Also run `lsusb | grep df11` — if it shows `0483:df11`, the board is sitting in DFU mode again.

---

## Hypothesis B: The HSE crystal frequency is wrong or the crystal is dead

Your board might not have a 25 MHz crystal. Cheap clones sometimes use 8 MHz, or the crystal might be unpopulated / broken.

If the crystal is actually **8 MHz**, your new PLL settings produce a VCO of **61.44 MHz**, which is **below the STM32F411 minimum of 100 MHz**. The PLL never locks, and `SystemInit()` hangs forever at:
```c
while (!(RCC->CR & (1U << 25)));  // wait for PLL lock
```

If this is the case, even the old 8 MHz code should have worked... but maybe it also hung because of a different issue, or maybe the crystal is just dead and neither code works.

---

## Proposed plan

1. **First, try releasing BOOT0 and pressing RESET** (Hypothesis A).

2. **If that doesn't work**, change `SystemInit()` to use the **internal HSI oscillator (16 MHz)** instead of HSE. This completely bypasses the crystal and will tell us immediately if the crystal is the problem.

   We can either:
   - Run at 16 MHz HSI directly (simplest, no PLL needed).
   - Or use HSI + PLL to get a higher speed.

3. **If HSI works**, we know your board either lacks the HSE crystal or has a different one. We can then figure out the correct frequency or just stick with HSI.

---

**Please try releasing BOOT0 and pressing RESET first, then let me know what happens.** If still no blink, I'll switch the clock setup to HSI so we can isolate the crystal issue.

---

