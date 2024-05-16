use crate::{
    enums::{Player, Terrain}, game_machine::GameMachine, machine::Machine
};

pub struct Game<'a> {
    pub opponent_starting_pieces: Vec<Machine>,
    pub player_starting_pieces: Vec<Machine>,
    pub board: [[Terrain; 8]; 8],
    pub machines: [[Option<&'a GameMachine>; 8]; 8],
    pub turn: Player,
}
