--Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2020.2 (lin64) Build 3064766 Wed Nov 18 09:12:47 MST 2020
--Date        : Fri Jul  9 21:48:13 2021
--Host        : id5014 running 64-bit Ubuntu 18.04.4 LTS
--Command     : generate_target mb_es_design_wrapper.bd
--Design      : mb_es_design_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity mb_es_design_wrapper is
  port (
    gt_refclk_n : in STD_LOGIC;
    gt_refclk_p : in STD_LOGIC;
    sfp_tx_disable : out STD_LOGIC_VECTOR ( 1 downto 0 );
    sys_reset : in STD_LOGIC;
    user_si570_sysclk_clk_n : in STD_LOGIC;
    user_si570_sysclk_clk_p : in STD_LOGIC
  );
end mb_es_design_wrapper;

architecture STRUCTURE of mb_es_design_wrapper is
  component mb_es_design is
  port (
    gt_refclk_n : in STD_LOGIC;
    gt_refclk_p : in STD_LOGIC;
    sys_reset : in STD_LOGIC;
    sfp_tx_disable : out STD_LOGIC_VECTOR ( 1 downto 0 );
    user_si570_sysclk_clk_n : in STD_LOGIC;
    user_si570_sysclk_clk_p : in STD_LOGIC
  );
  end component mb_es_design;
begin
mb_es_design_i: component mb_es_design
     port map (
      gt_refclk_n => gt_refclk_n,
      gt_refclk_p => gt_refclk_p,
      sfp_tx_disable(1 downto 0) => sfp_tx_disable(1 downto 0),
      sys_reset => sys_reset,
      user_si570_sysclk_clk_n => user_si570_sysclk_clk_n,
      user_si570_sysclk_clk_p => user_si570_sysclk_clk_p
    );
end STRUCTURE;
