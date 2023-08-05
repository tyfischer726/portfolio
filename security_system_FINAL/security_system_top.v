`include "debounce_filter.v"
`include "toggle_input.v"
`include "input_states.v"
`include "system_states.v"

module security_system_top (output wire o_LED_1, o_LED_2, o_LED_3, o_LED_4,
                           input wire i_Switch_1, i_Switch_2, i_Switch_4, i_Clk);
  
  wire [1:0] t_input_states;
  reg [1:0] r_input_states;
  
  input_states INPUTS(.in_Switch_1(i_Switch_1), .in_Switch_2(i_Switch_2), .i_Reset(i_Switch_4), .i_Clk(i_Clk),
                      .o_LED_3(o_LED_3), .o_LED_4(o_LED_4), .o_input_states(t_input_states));

    always @ (posedge i_Clk)
        begin
            r_input_states <= t_input_states;
        end

  system_states SYSTEM(.input_states(r_input_states), .i_Reset(i_Switch_4), .i_Clk(i_Clk), .o_LED_1(o_LED_1), .o_LED_2(o_LED_2));
  
endmodule