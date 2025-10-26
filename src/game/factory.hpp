#pragma once

#include <functional>
#include <map>
#include <memory>
#include <variant>

#include "../gfx/renderer.hpp"
#include "../krystl.hpp"

namespace game {
struct block_position {
    i32 x;
    i32 y;

    constexpr std::strong_ordering operator<=>(const block_position& other
    ) const {
        if (x < other.x) {
            return std::strong_ordering::less;
        }
        if (x > other.x) {
            return std::strong_ordering::greater;
        }
        if (y < other.y) {
            return std::strong_ordering::less;
        }
        if (y > other.y) {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }
};
class block {
public:
    constexpr virtual fn size() const -> u8 { return 1; }
    constexpr virtual fn color() const -> gfx::color {
        return {255, 0, 0, 255};
    }
};
class big_block : public block {
public:
    constexpr fn size() const -> u8 override { return 2; }
    constexpr fn color() const -> gfx::color override {
        return {0, 0, 255, 255};
    }
};
class factory {
private:
    std::map<block_position, std::unique_ptr<block>> blocks;
    std::map<block_position, block_position> top_left_references;

    // TODO: split
public:
    fn draw() {
        for (let & [ position, block ] : blocks) {
            let size = (f32)block->size() / 2;
            gfx::draw_square(
                0,
                {(f32)position.x + size, (f32)position.y + size},
                {(f32)size, (f32)size},
                block->color()
            );
        }
    }
    fn fits(const block& block, block_position at) -> bool {
        let size = block.size();
        for (u8 dy = 0; dy < size; dy++) {
            for (u8 dx = 0; dx < size; dx++) {
                if (top_left_references.contains({at.x + dx, at.y + dy})) {
                    return false;
                }
            }
        }
        return true;
    }
    fn place(std::unique_ptr<block> new_block, block_position at)
        -> kstl::option<std::unique_ptr<block>> {
        if (!fits(*new_block, at)) {
            return kstl::some(std::move(new_block));
        }
        let size = new_block->size();
        for (u8 dy = 0; dy < size; dy++) {
            for (u8 dx = 0; dx < size; dx++) {
                top_left_references.emplace(
                    block_position{at.x + dx, at.y + dy}, at
                );
            }
        }
        blocks.emplace(at, std::move(new_block));
        return kstl::none;
    }
    // TODO: return the old element if possible
    fn destroy(block_position at) {
        if (top_left_references.contains(at)) {
            let origin = top_left_references[at];
            let size = blocks[origin]->size();
            for (u8 dy = 0; dy < size; dy++) {
                for (u8 dx = 0; dx < size; dx++) {
                    top_left_references.erase(
                        block_position{origin.x + dx, origin.y + dy}
                    );
                }
            }
            blocks.erase(origin);
        }
    }
};
} // namespace game

namespace std {
template <> struct hash<game::block_position> {
    size_t operator()(const game::block_position& position) {
        let hasher = hash<i32>();
        return hasher(position.x) ^ hasher(position.y);
    }
};
} // namespace std
