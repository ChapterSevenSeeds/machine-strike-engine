use crate::{enums::{Terrain, Turn}, machine::Machine};

pub struct Game {
    pub opponent_starting_pieces: Vec<Machine>,
    pub player_starting_pieces: Vec<Machine>,
    pub board: [[Terrain; 8]; 8],
    pub machines: [[Option<Machine>; 8]; 8],
    pub turn: Turn
}