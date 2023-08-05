module debounce_filter (input i_Clk, input i_switch,
                        output f_switch);
 
  parameter c_DEBOUNCE_LIMIT = 250000;  // 10 ms at 25 MHz
   
  reg [17:0] r_Count = 0;
  reg r_State = 1'b0;
 
  always @(posedge i_Clk)
  begin
    // Switch input is different than internal switch value, so an input is
    // changing.  Increase the counter until it is stable for enough time.  
    if (i_switch !== r_State && r_Count < c_DEBOUNCE_LIMIT)
      r_Count <= r_Count + 1;
 
    // End of counter reached, switch is stable, register it, reset counter
    else if (r_Count == c_DEBOUNCE_LIMIT)
    begin
      r_State <= i_switch;
      r_Count <= 0;
    end 
 
    // Switches are the same state, reset the counter
    else
      r_Count <= 0;
  end
 
  // Assign internal register to output (debounced!)
  assign f_switch = r_State;
 
endmodule