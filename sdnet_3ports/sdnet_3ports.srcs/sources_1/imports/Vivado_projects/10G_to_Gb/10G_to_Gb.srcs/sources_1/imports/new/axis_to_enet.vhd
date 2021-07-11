----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Diogo Marques d.marques@ua.pt
-- 
-- Create Date: 04/26/2021 10:44:56 AM
-- Design Name: 
-- Module Name: axis_to_enet - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity axis_to_enet is
    Port ( tx_r_data_rdy : out STD_LOGIC;
           tx_r_rd : in STD_LOGIC;
           tx_r_valid : out STD_LOGIC;
           tx_r_data : out STD_LOGIC_VECTOR (7 downto 0);
           tx_r_sop : out STD_LOGIC;
           tx_r_eop : out STD_LOGIC;
           tx_r_err : out STD_LOGIC;
           tx_r_underflow : out STD_LOGIC;
           tx_r_flushed : out STD_LOGIC;
           tx_r_control : out STD_LOGIC;
           s_axis_tdata : in STD_LOGIC_VECTOR (7 downto 0);
           s_axis_tlast : in STD_LOGIC;
           s_axis_tready : out STD_LOGIC;
           s_axis_tvalid : in STD_LOGIC;
           s_axis_tkeep : in STD_LOGIC;
           s_axis_aclk : in STD_LOGIC;
           currrent_state : out std_logic_vector(2 downto 0);
           dma_tx_end_tog : in STD_LOGIC;
           dma_tx_status_tog : out STD_LOGIC;
           reset : in STD_LOGIC);
end axis_to_enet;

architecture Behavioral of axis_to_enet is

signal packet_finish : std_logic := '1';
signal packet_valid : std_logic := '0';

-- Build an enumerated type for the state machine
type state_type is (s0, s1, s2, s3, s4);

-- Register to hold the current state
signal state   : state_type;

begin
    
    -- Logic to advance to the next state
	process (s_axis_aclk, reset)
	begin
		if reset = '1' then
			state <= s0;
		elsif (rising_edge(s_axis_aclk)) then
			case state is
				when s0=>
					if s_axis_tvalid = '1' then
						state <= s1;
					else
						state <= s0;
					end if;
				when s1=>
					if tx_r_rd = '1' then
						state <= s2;
					else
						state <= s1;
					end if;
				when s2=>
					if tx_r_rd = '1' and s_axis_tvalid = '1'  then
						state <= s3;
					else
						state <= s2;
					end if;
				when s3 =>
					if s_axis_tlast = '1' then
						state <= s0;
					elsif tx_r_rd = '0' then 
					   state <= s4;
					else
						state <= s3;
					end if;
				when s4 =>
					if s_axis_tlast = '1' then
						state <= s0;
					elsif tx_r_rd = '1' then 
					   state <= s3;
					else
						state <= s4;
					end if;
			end case;
		end if;
	end process;
	
	-- Output depends solely on the current state
	process (state)
	begin
	
		case state is
			when s0 =>
			     currrent_state <= "000";
				tx_r_valid <= '0';
				tx_r_data_rdy <= '0';
				s_axis_tready <= '0';
				packet_valid <= '0';
			when s1 =>
			     currrent_state <= "001";
				tx_r_valid <= '0';
				tx_r_data_rdy <= '1';
				s_axis_tready <= '0';
				packet_valid <= '0';
			when s2 =>
			     currrent_state <= "010";
				tx_r_valid <= '0';
				tx_r_data_rdy <= '1';
				s_axis_tready <= '0';
				packet_valid <= '0';
			when s3 =>
			     if packet_finish = '1' and tx_r_rd = '1' then
			         tx_r_sop <= '1';
			     else
			         tx_r_sop <= '0';
			     end if;
			     currrent_state <= "011";
			     if (s_axis_tvalid = '1') then
			         tx_r_valid <= '1';
			         s_axis_tready <= '1';
			     else
			         tx_r_valid <= '0';
			         s_axis_tready <= '0';
			     end if;
				tx_r_data_rdy <= '0';
				--s_axis_tready <= tx_r_rd;
				packet_valid <= s_axis_tvalid;
			when s4 =>
			 currrent_state <= "100";
				tx_r_valid <= '0';
				tx_r_data_rdy <= '0';
				s_axis_tready <= '0';
				--s_axis_tready <= tx_r_rd;
				packet_valid <= s_axis_tvalid;
		end case;
	end process;
    
    
    tx_r_data <= s_axis_tdata;
    tx_r_eop <= s_axis_tlast;
    tx_r_err <= '0';
    tx_r_underflow <= '0';
    tx_r_flushed <= '0';
    tx_r_control <= '0';

    
    
    
    process (s_axis_aclk)
    begin
        if rising_edge(s_axis_aclk) then
            if(dma_tx_end_tog = '1') then
                dma_tx_status_tog <= '1';
            else
                dma_tx_status_tog <= '0';
            end if;
            
            if(s_axis_tlast = '1')then
                packet_finish <= '1';
            end if;
            
            if(packet_finish = '1' and packet_valid = '1')then
                packet_finish <= '0';
            end if;
            
        end if;
    end process;    


end Behavioral;
