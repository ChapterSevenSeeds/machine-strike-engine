use machine::Machine;
use machines::STORMBIRD;

use crate::enums::Terrain;

pub mod machine;
pub mod enums;
pub mod machines;
pub mod game;
pub mod moves;
pub mod game_machine;


fn main() {
    let mut board: [[Terrain; 8]; 8] = [
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland],
        [Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland]
    ];

    let mut machines: [[Option<Machine>; 8]; 8] = [
        [None, Some(STORMBIRD), None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None]
    ];

    print_board(board, machines);
}

fn print_board(board: [[Terrain; 8]; 8], machines: [[Option<Machine>; 8]; 8]) {
    println!("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    for row in 0..8 {
        for col in 0..8 {
            let machine = &machines[row][col];

            print!("|");
            match machine {
                Some(m) => {
                    print!("{name:<24}", name=m.name);
                },
                None => {
                    print!("                        ");
                }
            }
        }

        println!("|");

        for col in 0..8 {
            let terrain = &board[row][col];

            print!("|");
            print!("{:<24}", terrain.to_string());
        }

        println!("|");
        println!("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    }
}
