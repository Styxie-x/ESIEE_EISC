library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity top_counter is
    Generic (
        -- 100_000_000 cycles à 100 MHz = 1 seconde (1 Hz) pour le comptage.
        -- 100_000 cycles à 100 MHz = 1 ms (1 kHz) pour le balayage des 2 afficheurs.
        -- (En simulation, réduire via generic map, ex: MAX_COUNT => 10, REFRESH_MAX => 2)
        MAX_COUNT   : integer := 100_000_000;
        REFRESH_MAX : integer := 100_000
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

    -- Signaux pour la cadence 1 Hz (incrémentation)
    signal div_cnt    : integer range 0 to MAX_COUNT - 1 := 0;
    signal enable_1hz : std_logic := '0';

    -- Signaux des deux compteurs BCD (unités et dizaines)
    signal carry_unit : std_logic;
    signal bcd_unit   : std_logic_vector(3 downto 0);
    signal bcd_ten    : std_logic_vector(3 downto 0);
    signal reset_16   : std_logic;

    -- Signaux pour le multiplexage rapide entre AN0 et AN1
    signal ref_cnt    : integer range 0 to REFRESH_MAX - 1 := 0;
    signal mux_sel    : std_logic := '0';
    signal bcd_mux    : std_logic_vector(3 downto 0);

begin

    -- 1. Diviseur de fréquence 1 Hz (incrémentation toutes les 1 s)
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

    -- 2. Remise à zéro globale : bouton reset OU débordement à 15 -> 00
    reset_16 <= '1' when (reset = '1' or (bcd_ten = "0001" and bcd_unit = "0101" and enable_1hz = '1')) else '0';

    -- 3. Cascade des 2 compteurs BCD (Unités et Dizaines)
    u_counter : bcd0to9_counter port map (clk, reset_16, enable_1hz, carry_unit, bcd_unit);
    d_counter : bcd0to9_counter port map (clk, reset_16, carry_unit, open, bcd_ten);

    -- 4. Diviseur rapide pour le balayage des 2 afficheurs (~1 kHz)
    process(clk, reset)
    begin
        if reset = '1' then
            ref_cnt <= 0;
            mux_sel <= '0';
        elsif rising_edge(clk) then
            if ref_cnt = REFRESH_MAX - 1 then
                ref_cnt <= 0;
                mux_sel <= not mux_sel;
            else
                ref_cnt <= ref_cnt + 1;
            end if;
        end if;
    end process;

    -- 5. Multiplexage temporel : sélection du chiffre et de l'anode active (AN0 ou AN1)
    bcd_mux <= bcd_unit when mux_sel = '0' else bcd_ten;
    an      <= "11111110" when mux_sel = '0' else "11111101";

    -- 6. Décodage BCD vers 7 segments
    u_decoder : bcd_to_7seg port map (bcd_mux, seg);

end Behavioral;