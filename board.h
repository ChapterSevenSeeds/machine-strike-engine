#include "types.h"
#include <optional>
#include <functional>
#include "enums.h"
#include "game_machine.h"

class Board
{
public:
    BoardType<Terrain> terrain;
    BoardType<std::optional<std::reference_wrapper<GameMachine>>> machines;

    void unsafe_move_machine(int32_t source_row, int32_t source_column, int32_t destination_row, int32_t destination_column);
    bool is_space_occupied(int32_t row, int32_t column) const;
};