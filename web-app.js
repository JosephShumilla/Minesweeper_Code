const TILE_SIZE = 32;
const HEADER_HEIGHT = 100;
const DIGIT_WIDTH = 21;
const DIGIT_HEIGHT = 32;
const ASSETS = [
  "tile_hidden",
  "tile_revealed",
  "flag",
  "mine",
  "debug",
  "play",
  "pause",
  "face_happy",
  "face_lose",
  "face_win",
  "number_1",
  "number_2",
  "number_3",
  "number_4",
  "number_5",
  "number_6",
  "number_7",
  "number_8",
];

const digitsImage = new Image();
const images = {};

const canvas = document.getElementById("board");
const ctx = canvas.getContext("2d");

let config = { width: 25, height: 16, mines: 50 };
let state = {
  board: [],
  width: 0,
  height: 0,
  mines: 0,
  flags: 0,
  revealedSafe: 0,
  gameOver: false,
  gameWon: false,
  debug: false,
  paused: false,
  startTime: 0,
  pauseStart: 0,
  pausedDuration: 0,
  elapsed: 0,
};

const buttons = {
  reset: { x: 0, y: 0, w: 64, h: 64, name: "face_happy" },
  debug: { x: 0, y: 0, w: 64, h: 64, name: "debug" },
  pause: { x: 0, y: 0, w: 64, h: 64, name: "pause" },
};

function parseConfig(text) {
  const [width, height, mines] = text
    .trim()
    .split(/\s+/)
    .map((v) => Number.parseInt(v, 10));
  if (Number.isFinite(width) && Number.isFinite(height) && Number.isFinite(mines)) {
    return { width, height, mines };
  }
  return config;
}

async function loadConfig() {
  try {
    const res = await fetch("config.cfg");
    if (!res.ok) throw new Error("config not found");
    const text = await res.text();
    config = parseConfig(text);
  } catch (err) {
    console.warn("Falling back to default config", err);
  }
}

function loadAssets() {
  const promises = ASSETS.map(
    (name) =>
      new Promise((resolve, reject) => {
        const img = new Image();
        img.src = `images/${name}.png`;
        img.onload = () => {
          images[name] = img;
          resolve();
        };
        img.onerror = reject;
      })
  );

  const digitsPromise = new Promise((resolve, reject) => {
    digitsImage.src = "images/digits.png";
    digitsImage.onload = resolve;
    digitsImage.onerror = reject;
  });

  return Promise.all([...promises, digitsPromise]);
}

function createBoard(width, height) {
  return Array.from({ length: height }, () =>
    Array.from({ length: width }, () => ({
      mine: false,
      flagged: false,
      revealed: false,
      adjacent: 0,
    }))
  );
}

function shuffle(array) {
  for (let i = array.length - 1; i > 0; i -= 1) {
    const j = Math.floor(Math.random() * (i + 1));
    [array[i], array[j]] = [array[j], array[i]];
  }
  return array;
}

function placeMines(board, mines) {
  const height = board.length;
  const width = board[0].length;
  const positions = shuffle(
    Array.from({ length: width * height }, (_, i) => i)
  ).slice(0, mines);

  positions.forEach((pos) => {
    const x = pos % width;
    const y = Math.floor(pos / width);
    board[y][x].mine = true;
  });
}

function countAdjacent(board, x, y) {
  let count = 0;
  for (let dy = -1; dy <= 1; dy += 1) {
    for (let dx = -1; dx <= 1; dx += 1) {
      if (dx === 0 && dy === 0) continue;
      const nx = x + dx;
      const ny = y + dy;
      if (ny >= 0 && ny < board.length && nx >= 0 && nx < board[0].length) {
        if (board[ny][nx].mine) count += 1;
      }
    }
  }
  return count;
}

function computeAdjacency(board) {
  for (let y = 0; y < board.length; y += 1) {
    for (let x = 0; x < board[0].length; x += 1) {
      board[y][x].adjacent = countAdjacent(board, x, y);
    }
  }
}

function resetState() {
  state.width = config.width;
  state.height = config.height;
  state.mines = config.mines;
  state.board = createBoard(config.width, config.height);
  placeMines(state.board, config.mines);
  computeAdjacency(state.board);
  state.flags = 0;
  state.revealedSafe = 0;
  state.gameOver = false;
  state.gameWon = false;
  state.debug = false;
  state.paused = false;
  state.startTime = performance.now();
  state.pausedDuration = 0;
  state.elapsed = 0;
  sizeCanvas();
  updateButtonPositions();
}

function sizeCanvas() {
  canvas.width = state.width * TILE_SIZE;
  canvas.height = state.height * TILE_SIZE + HEADER_HEIGHT;
}

function updateButtonPositions() {
  const baseY = state.height * TILE_SIZE + 16;
  const centerX = (state.width * TILE_SIZE) / 2 - buttons.reset.w / 2;
  buttons.reset.x = centerX;
  buttons.reset.y = baseY - 16;

  buttons.debug.x = state.width * TILE_SIZE - 240;
  buttons.debug.y = baseY - 16;

  buttons.pause.x = state.width * TILE_SIZE - 176;
  buttons.pause.y = baseY - 16;
}

function toggleFlag(x, y) {
  const cell = state.board[y][x];
  if (cell.revealed || state.gameOver || state.gameWon || state.paused) return;
  cell.flagged = !cell.flagged;
  state.flags += cell.flagged ? 1 : -1;
}

function reveal(x, y) {
  const cell = state.board[y][x];
  if (cell.revealed || cell.flagged || state.gameOver || state.gameWon || state.paused) return;
  cell.revealed = true;
  if (cell.mine) {
    state.gameOver = true;
    state.elapsed = elapsedSeconds();
    return;
  }
  state.revealedSafe += 1;
  if (cell.adjacent === 0) {
    floodReveal(x, y);
  }
  checkWin();
}

function floodReveal(x, y) {
  const queue = [[x, y]];
  while (queue.length) {
    const [cx, cy] = queue.shift();
    for (let dy = -1; dy <= 1; dy += 1) {
      for (let dx = -1; dx <= 1; dx += 1) {
        if (dx === 0 && dy === 0) continue;
        const nx = cx + dx;
        const ny = cy + dy;
        if (ny < 0 || ny >= state.height || nx < 0 || nx >= state.width) continue;
        const neighbor = state.board[ny][nx];
        if (neighbor.revealed || neighbor.flagged) continue;
        neighbor.revealed = true;
        if (!neighbor.mine) state.revealedSafe += 1;
        if (neighbor.adjacent === 0 && !neighbor.mine) {
          queue.push([nx, ny]);
        }
      }
    }
  }
}

function checkWin() {
  if (state.revealedSafe === state.width * state.height - state.mines) {
    state.gameWon = true;
    state.elapsed = elapsedSeconds();
  }
}

function elapsedSeconds() {
  if (state.gameOver || state.gameWon) return state.elapsed;
  const now = performance.now();
  const delta = state.paused
    ? state.pauseStart - state.startTime - state.pausedDuration
    : now - state.startTime - state.pausedDuration;
  return Math.floor(delta / 1000);
}

function togglePause() {
  if (state.gameOver || state.gameWon) return;
  state.paused = !state.paused;
  if (state.paused) {
    state.pauseStart = performance.now();
  } else {
    state.pausedDuration += performance.now() - state.pauseStart;
  }
}

function toggleDebug() {
  if (state.gameOver || state.gameWon) return;
  state.debug = !state.debug;
}

function drawTile(x, y, cell) {
  const dx = x * TILE_SIZE;
  const dy = y * TILE_SIZE;
  const hiddenImg = images.tile_hidden;
  const revealedImg = images.tile_revealed;

  if (state.paused) {
    ctx.drawImage(hiddenImg, dx, dy, TILE_SIZE, TILE_SIZE);
    return;
  }

  if (!cell.revealed) {
    ctx.drawImage(hiddenImg, dx, dy, TILE_SIZE, TILE_SIZE);
  } else {
    ctx.drawImage(revealedImg, dx, dy, TILE_SIZE, TILE_SIZE);
  }

  if (cell.revealed && cell.mine) {
    ctx.drawImage(images.mine, dx, dy, TILE_SIZE, TILE_SIZE);
  } else if (cell.revealed && cell.adjacent > 0) {
    const numImg = images[`number_${cell.adjacent}`];
    ctx.drawImage(numImg, dx, dy, TILE_SIZE, TILE_SIZE);
  }

  if (cell.flagged && !cell.revealed) {
    ctx.drawImage(images.flag, dx, dy, TILE_SIZE, TILE_SIZE);
  }

  if (!cell.revealed && state.debug && cell.mine) {
    ctx.drawImage(images.mine, dx, dy, TILE_SIZE, TILE_SIZE);
  }
}

function drawDigits(value, digits, x, y) {
  const capped = Math.max(Math.min(value, 9999), -999);
  const str = capped.toString().padStart(digits, "0").slice(-digits);
  for (let i = 0; i < str.length; i += 1) {
    const ch = str[i];
    const idx = ch === "-" ? 10 : Number.parseInt(ch, 10);
    ctx.drawImage(
      digitsImage,
      idx * DIGIT_WIDTH,
      0,
      DIGIT_WIDTH,
      DIGIT_HEIGHT,
      x + i * DIGIT_WIDTH,
      y,
      DIGIT_WIDTH,
      DIGIT_HEIGHT
    );
  }
}

function drawHeader() {
  const y = state.height * TILE_SIZE;
  ctx.fillStyle = "#c0d6ec";
  ctx.fillRect(0, y, canvas.width, HEADER_HEIGHT);

  const bombsLeft = state.mines - state.flags;
  drawDigits(bombsLeft, 4, 12, y + 8);

  const timer = elapsedSeconds();
  drawDigits(timer, 4, canvas.width - DIGIT_WIDTH * 4 - 12, y + 8);

  const face = state.gameOver
    ? images.face_lose
    : state.gameWon
    ? images.face_win
    : images.face_happy;
  const reset = buttons.reset;
  ctx.drawImage(face, reset.x, reset.y, reset.w, reset.h);
  ctx.drawImage(images.debug, buttons.debug.x, buttons.debug.y, buttons.debug.w, buttons.debug.h);
  const pauseImg = state.paused ? images.play : images.pause;
  ctx.drawImage(pauseImg, buttons.pause.x, buttons.pause.y, buttons.pause.w, buttons.pause.h);
}

function draw() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  for (let y = 0; y < state.height; y += 1) {
    for (let x = 0; x < state.width; x += 1) {
      drawTile(x, y, state.board[y][x]);
    }
  }
  drawHeader();
}

function findButton(mx, my) {
  return Object.entries(buttons).find(([, btn]) =>
    mx >= btn.x && mx <= btn.x + btn.w && my >= btn.y && my <= btn.y + btn.h
  );
}

function handleLeftClick(mx, my) {
  if (my >= state.height * TILE_SIZE) {
    const hit = findButton(mx, my);
    if (hit) {
      const [name] = hit;
      if (name === "reset") {
        resetState();
      } else if (name === "debug") {
        toggleDebug();
      } else if (name === "pause") {
        togglePause();
      }
      return;
    }
    return;
  }

  const x = Math.floor(mx / TILE_SIZE);
  const y = Math.floor(my / TILE_SIZE);
  reveal(x, y);
}

function handleRightClick(mx, my) {
  if (my >= state.height * TILE_SIZE) return;
  const x = Math.floor(mx / TILE_SIZE);
  const y = Math.floor(my / TILE_SIZE);
  toggleFlag(x, y);
}

canvas.addEventListener("contextmenu", (e) => e.preventDefault());
canvas.addEventListener("mousedown", (e) => {
  const rect = canvas.getBoundingClientRect();
  const scaleX = canvas.width / rect.width;
  const scaleY = canvas.height / rect.height;
  const mx = (e.clientX - rect.left) * scaleX;
  const my = (e.clientY - rect.top) * scaleY;
  if (e.button === 0) {
    handleLeftClick(mx, my);
  } else if (e.button === 2) {
    handleRightClick(mx, my);
  }
  draw();
});

function gameLoop() {
  if (!state.paused && !state.gameOver && !state.gameWon) {
    state.elapsed = elapsedSeconds();
  }
  draw();
  requestAnimationFrame(gameLoop);
}

async function init() {
  await loadConfig();
  await loadAssets();
  resetState();
  draw();
  requestAnimationFrame(gameLoop);
}

init();
