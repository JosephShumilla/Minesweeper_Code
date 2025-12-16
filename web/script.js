const boardElement = document.getElementById("board");
const mineCountElement = document.getElementById("mine-count");
const timerElement = document.getElementById("timer");
const gameStateElement = document.getElementById("game-state");
const widthInput = document.getElementById("width");
const heightInput = document.getElementById("height");
const minesInput = document.getElementById("mines");
const restartButton = document.getElementById("restart");

const DEFAULT_CONFIG = {
  width: 25,
  height: 16,
  mines: 50,
};

let board = [];
let width = DEFAULT_CONFIG.width;
let height = DEFAULT_CONFIG.height;
let mines = DEFAULT_CONFIG.mines;
let revealed = 0;
let flagged = 0;
let isGameOver = false;
let timerId = null;
let startTime = null;

function clamp(value, min, max) {
  return Math.min(Math.max(value, min), max);
}

function resetTimer() {
  if (timerId) {
    clearInterval(timerId);
  }
  startTime = Date.now();
  timerId = setInterval(() => {
    const elapsed = Math.floor((Date.now() - startTime) / 1000);
    timerElement.textContent = `Time: ${elapsed}s`;
  }, 500);
}

function shuffle(array) {
  for (let i = array.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [array[i], array[j]] = [array[j], array[i]];
  }
}

function buildBoard() {
  width = clamp(Number(widthInput.value) || DEFAULT_CONFIG.width, 5, 35);
  height = clamp(Number(heightInput.value) || DEFAULT_CONFIG.height, 5, 25);
  mines = clamp(Number(minesInput.value) || DEFAULT_CONFIG.mines, 1, width * height - 1);

  widthInput.value = width;
  heightInput.value = height;
  minesInput.value = mines;

  board = Array.from({ length: height }, (_, y) =>
    Array.from({ length: width }, (_, x) => ({
      x,
      y,
      hasMine: false,
      revealed: false,
      flagged: false,
      adjacent: 0,
      element: null,
    }))
  );

  placeMines();
  calculateAdjacents();
  renderBoard();
  revealed = 0;
  flagged = 0;
  isGameOver = false;
  gameStateElement.textContent = "";
  updateMineCount();
  resetTimer();
}

function placeMines() {
  const positions = [];
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      positions.push({ x, y });
    }
  }
  shuffle(positions);
  positions.slice(0, mines).forEach(({ x, y }) => {
    board[y][x].hasMine = true;
  });
}

function forEachNeighbor(x, y, callback) {
  for (let dy = -1; dy <= 1; dy++) {
    for (let dx = -1; dx <= 1; dx++) {
      if (dx === 0 && dy === 0) continue;
      const nx = x + dx;
      const ny = y + dy;
      if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        callback(board[ny][nx]);
      }
    }
  }
}

function calculateAdjacents() {
  board.forEach((row) => {
    row.forEach((tile) => {
      let count = 0;
      forEachNeighbor(tile.x, tile.y, (neighbor) => {
        if (neighbor.hasMine) count++;
      });
      tile.adjacent = count;
    });
  });
}

function updateMineCount() {
  const remaining = Math.max(mines - flagged, 0);
  mineCountElement.textContent = `Mines: ${remaining}/${mines}`;
}

function revealTile(tile) {
  if (tile.revealed || tile.flagged || isGameOver) return;
  tile.revealed = true;
  revealed++;
  tile.element.classList.add("revealed");

  if (tile.hasMine) {
    tile.element.classList.add("mine");
    tile.element.textContent = "💥";
    endGame(false);
    return;
  }

  if (tile.adjacent > 0) {
    tile.element.textContent = tile.adjacent;
    tile.element.classList.add(`num-${tile.adjacent}`);
  } else {
    tile.element.classList.add("zero");
    floodReveal(tile);
  }

  checkWin();
}

function floodReveal(startTile) {
  const queue = [startTile];
  const visited = new Set();

  while (queue.length) {
    const tile = queue.shift();
    const key = `${tile.x}-${tile.y}`;
    if (visited.has(key)) continue;
    visited.add(key);

    if (tile.adjacent === 0 && !tile.hasMine) {
      forEachNeighbor(tile.x, tile.y, (neighbor) => {
        if (!neighbor.revealed && !neighbor.flagged) {
          neighbor.revealed = true;
          revealed++;
          neighbor.element.classList.add("revealed", neighbor.adjacent === 0 ? "zero" : "");
          if (neighbor.adjacent > 0) {
            neighbor.element.textContent = neighbor.adjacent;
            neighbor.element.classList.add(`num-${neighbor.adjacent}`);
          }
          queue.push(neighbor);
        }
      });
    }
  }
}

function toggleFlag(tile, event) {
  if (tile.revealed || isGameOver) return;
  event?.preventDefault();
  tile.flagged = !tile.flagged;
  flagged += tile.flagged ? 1 : -1;
  tile.element.classList.toggle("flagged", tile.flagged);
  tile.element.textContent = tile.flagged ? "🚩" : "";
  updateMineCount();
}

function endGame(won) {
  isGameOver = true;
  clearInterval(timerId);
  board.forEach((row) =>
    row.forEach((tile) => {
      if (tile.hasMine) {
        tile.element.textContent = tile.flagged ? "🚩" : "💣";
        tile.element.classList.add("revealed");
      }
    })
  );
  gameStateElement.textContent = won ? "You cleared the field!" : "Boom! Game over.";
  gameStateElement.style.color = won ? "var(--success)" : "var(--danger)";
}

function checkWin() {
  if (revealed === width * height - mines && !isGameOver) {
    endGame(true);
  }
}

function createCell(tile) {
  const cell = document.createElement("button");
  cell.type = "button";
  cell.className = "cell";
  cell.setAttribute("role", "gridcell");
  cell.setAttribute("aria-label", `Tile ${tile.x + 1}, ${tile.y + 1}`);

  cell.addEventListener("click", () => revealTile(tile));
  cell.addEventListener("contextmenu", (event) => toggleFlag(tile, event));
  cell.addEventListener("pointerdown", (event) => {
    if (event.pointerType === "touch") {
      toggleFlag(tile, event);
    }
  });

  tile.element = cell;
  return cell;
}

function renderBoard() {
  boardElement.style.gridTemplateColumns = `repeat(${width}, var(--cell-size))`;
  boardElement.innerHTML = "";
  board.forEach((row) => {
    row.forEach((tile) => {
      boardElement.appendChild(createCell(tile));
    });
  });
}

restartButton.addEventListener("click", buildBoard);

buildBoard();
