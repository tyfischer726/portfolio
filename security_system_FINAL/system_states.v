module system_states(output reg o_LED_1, o_LED_2,
                     input wire [1:0] input_states,
                    input wire i_Reset, i_Clk);
  
  reg [2:0] current_state, next_state;
  reg [1:0] r_input_states;
  
  parameter initial_state = 3'b000,
  			incorrect1 = 3'b001,
  			incorrect2 = 3'b010,
  			incorrect3 = 3'b011,
  			disarmed = 3'b100;

    always @ (posedge i_Clk)
    begin
        r_input_states <= input_states;
    end
  
  //State Memory Logic
  always @ (posedge i_Clk or posedge i_Reset)
    begin : STATE_MEMORY_LOGIC
      if (i_Reset)
        current_state <= 0;
      else
        current_state <= next_state;
    end
  
  
  //Next State Logic
  always @ (input_states or current_state)
    begin : NEXT_STATE_LOGIC
      if (i_Reset)
        next_state = initial_state;
      else if (r_input_states != input_states)
        begin
          case(current_state)
            initial_state		:	if (input_states == 2)
                                        next_state = incorrect1;
                                    else if (input_states == 3)
                                      next_state = disarmed;
                                    else
                                      next_state = current_state;
            incorrect1			:	if (input_states == 2)
                                        next_state = incorrect2;
                                    else if (input_states == 3)
                                      next_state = disarmed;
                                    else
                                      next_state = current_state;
            incorrect2			:	if (input_states == 2)
                                        next_state = incorrect3;
                                    else if (input_states == 3)
                                      next_state = disarmed;
                                    else
                                      next_state = current_state;
            incorrect3			:	begin
                                        next_state = current_state;
                                    end
            disarmed			:	if (input_states == 3)
                                        next_state = initial_state;
                                    else
                                      next_state = current_state;
            default				:	begin
                                        next_state = initial_state;
                                    end
          endcase
        end
        else
            next_state = current_state;
    end
  
  
  //Output Logic
  always @ (current_state)
    begin : OUTPUT_LOGIC
      case(current_state)
        initial_state		:	begin
          							o_LED_1 = 0;
          							o_LED_2 = 0;
        						end
        incorrect1			:	begin
          							o_LED_1 = 0;
          							o_LED_2 = 1;
        						end
        incorrect2			:	begin
          							o_LED_1 = 0;
          							o_LED_2 = 1;
        						end
        incorrect3			:	begin
          							o_LED_1 = 0;
          							o_LED_2 = 1;
        						end
        disarmed			:	begin
          							o_LED_1 = 1;
          							o_LED_2 = 0;
        						end
        default				:	begin
          							o_LED_1 = 0;
          							o_LED_2 = 0;
        						end
      endcase
    end

endmodule