module toggle_input(output wire o_input_state,
                    input wire i_Switch, i_Reset, i_Clk);
  
  reg r_input_state = 0, r_Switch = 0;
  wire f_Switch;
  
  debounce_filter F_SWITCH(.i_Clk(i_Clk), .i_switch(i_Switch), .f_switch(f_Switch));
  
  always @(posedge i_Clk or posedge i_Reset)
  begin
    if (i_Reset)
        begin
            r_input_state <= 0;
            r_Switch <= 0;
        end
    else
        begin
            r_Switch <= f_Switch;
                if (f_Switch == 0 && r_Switch == 1)
                r_input_state <= ~r_input_state;
        end
  end
  
  assign o_input_state = r_input_state;
  
endmodule