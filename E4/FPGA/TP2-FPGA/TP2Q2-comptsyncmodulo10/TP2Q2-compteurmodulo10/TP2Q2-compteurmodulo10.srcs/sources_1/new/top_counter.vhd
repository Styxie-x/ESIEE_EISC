library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity top_counter is
    Generic (
        MAX_COUNT : integer := 100_000_000
    );
    Port ( 
        clk   : in  std_logic;
        reset : in  std_logic;
        seg   : out std_logic_vector(7 downto 0);
        an    : out std_logic_vector(7 downto 0)
    );
end top_counter;

architecture Behavioral of top_counter is

    component bcd0to9_counter is
        port(
            clk    : in  std_logic;
            reset  : in  std_logic;
            enable : in  std_logic;
            carry  : out std_logic;
            bcd4   : out std_logic_vector(3 downto 0)
        );
    end component;

    component bcd_to_7seg is
        port(
            B : in  std_logic_vector(3 downto 0);
            O : out std_logic_vector(7 downto 0)
        );
    end component;

    signal div_cnt    : integer range 0 to MAX_COUNT - 1 := 0;
    signal enable_1hz : std_logic := '0';
    signal bcd_val    : std_logic_vector(3 downto 0);

begin

    -- Diviseur de fréquence (1 Hz)
    process(clk, reset)
    begin
        if reset = '1' then
            div_cnt    <= 0;
            enable_1hz <= '0';
        elsif rising_edge(clk) then
            if div_cnt = MAX_COUNT - 1 then
                div_cnt    <= 0;
                enable_1hz <= '1';
            else
                div_cnt    <= div_cnt + 1;
                enable_1hz <= '0';
            end if;
        end if;
    end process;

    -- Instanciations sur une ligne en association positionnelle (ordre exact des ports)
    u_counter : bcd0to9_counter port map (clk, reset, enable_1hz, open, bcd_val);
    u_decoder : bcd_to_7seg port map (bcd_val, seg);

    -- Activation uniquement de l'afficheur de droite AN0 (logique active-basse)
    an <= "11111110";

end Behavioral;