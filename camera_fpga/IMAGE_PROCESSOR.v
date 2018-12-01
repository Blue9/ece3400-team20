`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144

module IMAGE_PROCESSOR (
	PIXEL_IN,
	CLK,
	VGA_PIXEL_X,
	VGA_PIXEL_Y,
	VSYNC,
	RESULT,
	SHAPE,
	HREF
);


//=======================================================
//  PORT declarations
//=======================================================
input	[7:0]	PIXEL_IN;
input 		CLK;
input [9:0] VGA_PIXEL_X;
input [9:0] VGA_PIXEL_Y;
input			VSYNC;
input			HREF;


output reg [1:0] RESULT;
output reg [7:0] SHAPE;
reg [15:0] countBLUE;
reg [15:0] countRED;
reg [15:0] countBLUE_1 = 16'd0;
reg [15:0] countBLUE_2 = 16'd0;
reg [15:0] countBLUE_3 = 16'd0;
reg [15:0] countRED_1  = 16'd0;
reg [15:0] countRED_2  = 16'd0;
reg [15:0] countRED_3  = 16'd0;

reg [15:0] count_1  = 16'd0;
reg [15:0] count_2  = 16'd0;
reg [15:0] count_3  = 16'd0;

localparam Y_ADDR1 = 8'd50;
localparam Y_ADDR2 = 8'd72;
localparam Y_ADDR3 = 8'd94;
localparam MARGIN = 8'd10;

reg [15:0] countNULL;
reg [15:0] R_CNT_THRESHOLD = 16'd70;
reg [15:0] B_CNT_THRESHOLD = 16'd70;
reg lastsync = 1'b0;
reg lasthref = 1'b0;
reg [175:0] first_row;
reg [175:0] middle_row;
reg [175:0] last_row;
reg [29:0] counter = 30'd225000000;

reg [9:0] pixel_x_prev;
always @(posedge CLK) begin
	if(VGA_PIXEL_X != pixel_x_prev) begin
	
		if (PIXEL_IN[4:2] == 3'b000) begin
			if(VGA_PIXEL_Y == Y_ADDR1) begin count_1 = count_1 + 16'd1; end
			if(VGA_PIXEL_Y == Y_ADDR2) begin count_2 = count_2 + 16'd1; end
			if(VGA_PIXEL_Y == Y_ADDR3) begin count_3 = count_3 + 16'd1; end
		end
		
		if(PIXEL_IN[1:0] == 2'b11) begin 
			countBLUE = countBLUE + 16'd1; 
		end
		else if(PIXEL_IN[7:5] == 3'b111) begin 
			countRED = countRED + 16'b1; 
		end
		else begin countNULL = countNULL + 16'd1; end

	end
	else if(VGA_PIXEL_Y > 130) begin //posedge VSYNC (end of frame - assign shapes & colors)
		if(countBLUE > B_CNT_THRESHOLD) begin 
			RESULT = 2'b10; 
		end
		if(countRED > R_CNT_THRESHOLD) begin RESULT = 2'b01; end
		else if (countBLUE <= B_CNT_THRESHOLD) begin RESULT = 2'b00; end

		// triangle
		if(count_3 - count_1 > 40) begin
			SHAPE = 8'b11100000;
		end
		// square
		else if (count_1 - count_3 < 10) begin
			SHAPE = 8'b00011100;
		end
		// diamond
		else if (count_2 - count_3 > 10) begin
			SHAPE = 8'b00000011;
		end
		else begin SHAPE = 8'd255; end
	end	
	else if(VGA_PIXEL_Y < 10) begin //negedge VSYNC (reset count)
		countBLUE = 16'b0;
		countRED = 16'b0;
		countNULL = 16'b0;
		count_1 = 0;
		count_2 = 0;
		count_3 = 0;
	end
	lastsync = VSYNC;
end

endmodule


