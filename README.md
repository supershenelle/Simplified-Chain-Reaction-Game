# 💥 Chain Reaction Game

> A two-player, console-based chain reaction board game written in C.

**Authors:** Emmanuel Gagasa, Shenelle Andrea Nono, Sabina Fornoles  
**Section:** S15 / S05

---

## 📋 Overview

Chain Reaction is a turn-based strategy game played on a **3×3 grid** between two players — **RED** and **BLUE**. Players take turns placing and expanding their pieces on the board. When a piece is stacked twice on the same cell, it **explodes** and chains into adjacent cells, capturing or converting neighboring pieces. The player with the most pieces when the game ends wins.

---

## 🎮 How to Play

1. **Placement Phase** — RED and BLUE take turns placing one piece on any empty cell until both have at least one piece on the board.
2. **Expansion Phase** — Players select one of their own pieces to expand (stack). A cell stacked **twice** triggers a chain reaction, spreading to all 4 adjacent cells.
3. **Chain Reactions** — Exploding pieces capture or convert adjacent opponent pieces, which may trigger further reactions.
4. **Game Ends** when any of the following conditions are met:
   - Only **3 free cells** remain
   - **20 moves** have been played
   - One player has **no pieces** left on the board

The player with the **most pieces** at the end wins. A **draw** is declared if both players have equal pieces.

---

## 🗂️ Project Structure

```
chain-reaction/
├── game.c       # Game mechanic functions
└── main.c       # Entry point (game loop)
```

---

## ⚙️ Core Functions

| Function | Description |
|----------|-------------|
| `initializeGame` | Sets up the board and resets all game state |
| `displayBoard` | Renders the 3×3 board with color-coded pieces |
| `getMove` | Validates and reads player input (row & column) |
| `isValidPos` | Checks if a position is within the 3×3 grid |
| `nextPlayerMove` | Processes a player's move and switches turns |
| `updatePos` | Stacks a piece; triggers expansion if stacked twice |
| `expandPos` | Explodes a cell, spreading to all 4 adjacent cells |
| `replacePos` | Captures or converts pieces at a target cell |
| `removePos` | Removes current player's piece from a cell |
| `countPieces` | Counts total pieces on a given board |
| `countFreeCells` | Returns number of unoccupied cells |
| `checkGameOver` | Evaluates end-game conditions |
| `showResult` | Displays final piece counts and declares the winner |

---

## 🧱 Game State

The `GameState` struct tracks all game data:

| Field | Description |
|-------|-------------|
| `R[4][4]` | RED player's pieces |
| `B[4][4]` | BLUE player's pieces |
| `S[4][4]` | Occupied cells (either player) |
| `T[4][4]` | Temporary expansion tracking |
| `go` | Current turn (`1` = RED, `0` = BLUE) |
| `val` | Move counter |
| `start` | Placement phase flag |
| `over` | Game over flag |

---

## 🖥️ Display

The board renders with **ANSI color codes** in the terminal:
- 🔴 **RED** pieces displayed in red
- 🔵 **BLUE** pieces displayed in blue
- Current turn and move count shown below the board

---

## 👩‍💻 Authors

| Name | Section |
|------|---------|
| Emmanuel Gagasa | S15 |
| Shenelle Andrea Nono | S15 |
| Sabina Fornoles | S05 |

---

<p align="center"><em>Built with C, chain reactions, and a few explosions. 💥</em></p>
