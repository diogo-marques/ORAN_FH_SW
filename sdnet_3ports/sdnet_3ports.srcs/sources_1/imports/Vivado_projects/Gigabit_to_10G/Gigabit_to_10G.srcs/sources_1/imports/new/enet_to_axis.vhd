----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Diogo Marques d.marques@ua.pt
-- 
-- Create Date: 04/23/2021 02:48:36 PM
-- Design Name: 
-- Module Name: enet_to_axis - Behavioral
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

entity enet_to_axis is
    Port ( rx_w_wr : in STD_LOGIC;
           rx_w_data : in STD_LOGIC_VECTOR (7 downto 0);
           rx_w_sop : in STD_LOGIC;
           rx_w_eop : in STD_LOGIC;
           rx_w_err : in STD_LOGIC;
           rx_w_overflow : out STD_LOGIC;
           rx_w_flush : in STD_LOGIC;
           m_axis_tdata : out STD_LOGIC_VECTOR (7 downto 0);
           m_axis_tlast : out STD_LOGIC;
           m_axis_tready : in STD_LOGIC;
           m_axis_tvalid : out STD_LOGIC;
           m_axis_tuser : out STD_LOGIC;
           m_axis_tkeep : out STD_LOGIC;
           m_axis_aclk : in STD_LOGIC
           );
end enet_to_axis;

architecture Behavioral of enet_to_axis is

begin
    
    --process (m_axis_aclk)
    --begin
    --   if rising_edge(m_axis_aclk) then
            m_axis_tdata <= rx_w_data;
            m_axis_tlast <= rx_w_eop;
            m_axis_tvalid <= rx_w_wr;
            m_axis_tuser <= '0';
            m_axis_tkeep <= '1';
    --    end if;
    --end process;

end Behavioral;
