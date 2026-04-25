module top (
    input  mode0_key1,
    output led0
);

    // mode0_key1 is active-low (pulled up internally).
    // Light led0 when the button is pressed.
    assign led0 = ~mode0_key1;

endmodule
