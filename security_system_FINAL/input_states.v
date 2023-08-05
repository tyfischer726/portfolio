module input_states(output reg [1:0] o_input_states,
                    output reg o_LED_3, o_LED_4,
                   input wire in_Switch_1, in_Switch_2, i_Reset, i_Clk);
  
  reg [1:0] current_state, next_state;
  wire w_Switch_1, w_Switch_2;
  
  parameter S0 = 2'b00,
  			S1 = 2'b01,
  			S2 = 2'b10,
  			S3 = 2'b11;
  
  toggle_input Switch1_State(.i_Clk(i_Clk), .i_Switch(in_Switch_1), .i_Reset(i_Reset), .o_input_state(w_Switch_1));
  toggle_input Switch2_State(.i_Clk(i_Clk), .i_Switch(in_Switch_2), .i_Reset(i_Reset), .o_input_state(w_Switch_2));
  
  //State Memory Logic
  always @ (posedge i_Clk or posedge i_Reset)
    begin
      if (i_Reset)
        current_state <= S0;
      else
        current_state <= next_state;
    end
  
  
  //Next State Logic
  always @ (w_Switch_1 or w_Switch_2 or current_state)
    begin
      if (w_Switch_1 == 1 && w_Switch_2 == 1)
        next_state = S3;
      else if (w_Switch_1 == 1 && w_Switch_2 == 0)
        next_state = S2;
      else if (w_Switch_1 == 0 && w_Switch_2 == 1)
        next_state = S1;
      else
        next_state = S0;
    end
  
  
  //Output Logic
  always @ (current_state)
    begin : OUTPUT_LOGIC
      case(current_state)
        S0			:	begin
          					o_input_states = 0;
          					o_LED_3 = 0;
          					o_LED_4 = 0;
        				end
        S1			:	begin
          					o_input_states = 1;
          					o_LED_3 = 0;
          					o_LED_4 = 1;
        				end
        S2			:	begin
          					o_input_states = 2;
          					o_LED_3 = 1;
          					o_LED_4 = 0;
        				end
        S3			:	begin
          					o_input_states = 3;
          					o_LED_3 = 1;
          					o_LED_4 = 1;
        				end
        default		:	begin
          					o_input_states = 0;
          					o_LED_3 = 0;
          					o_LED_4 = 0;
        				end
      endcase
    end
  
endmodule