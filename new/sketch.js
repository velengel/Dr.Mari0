/*
Dr. Mari0 - p5.js Remake

A complete remake of the C++ openFrameworks project in p5.js.
All game logic and p5.js code is consolidated into this single file for stability.
*/

// -----------------
// --- CONSTANTS ---
// -----------------
const CELL_SIZE = 40;
const BOARD_WIDTH = 8;
const BOARD_HEIGHT = 16;
const UI_WIDTH = 240;
const BOARD_PIXEL_WIDTH = BOARD_WIDTH * CELL_SIZE;
const CANVAS_WIDTH = BOARD_PIXEL_WIDTH + UI_WIDTH;
const CANVAS_HEIGHT = BOARD_HEIGHT * CELL_SIZE;
const FALL_SPEED_NORMAL = 30; // Frames per cell drop
const FALL_SPEED_FAST = 5;
const VIRUS_COUNT = 10;

const PILL_COLORS = {
  0: '#333333',
  1: '#FF4141', // Red
  2: '#FFFF00', // Yellow
  3: '#87CEEB', // Blue
};

// -----------------------------
// --- GAME STATE & GLOBALS  ---
// -----------------------------
let capsule;
let nextCapsule;
let grid;
let fallCounter = 0;
let fallSpeed = FALL_SPEED_NORMAL;
let gameState = 'PLAYING'; // PLAYING, GAME_OVER, CLEAR
let score = 0;
let sounds = {};
let images = {};

// -----------------------------------
// --- P5.JS LIFECYCLE FUNCTIONS ---
// -----------------------------------

function preload() {
    soundFormats('wav');
    sounds.bgm = loadSound('assets/sounds/vsvirus');
    sounds.move = loadSound('assets/sounds/move');
    sounds.rotate = loadSound('assets/sounds/rot');
    sounds.land = loadSound('assets/sounds/set');
    sounds.clear = loadSound('assets/sounds/erase');

    images.viruses = {
        1: loadImage('assets/images/rvirus.png'),
        2: loadImage('assets/images/yvirus.png'),
        3: loadImage('assets/images/bvirus.png'),
    };
}

function setup() {
  createCanvas(CANVAS_WIDTH, CANVAS_HEIGHT);
  console.log("Game starting...");
  
  initializeGrid();
  nextCapsule = generateRandomCapsule();
  spawnNewCapsule();

  sounds.bgm.loop();
  sounds.bgm.setVolume(0.3);
}

function draw() {
  background(51);
  drawGameBoard();
  drawGridContent();
  drawUI();
  
  if (gameState === 'PLAYING') {
    handleFalling();
    if (capsule) {
        drawCapsule();
    }
  } else if (gameState === 'GAME_OVER') {
    drawOverlay("GAME OVER");
  } else if (gameState === 'CLEAR') {
    drawOverlay("YOU WIN!", color(30, 180, 30, 150));
  }
}

function keyPressed() {
  if (gameState !== 'PLAYING' || !capsule) return;

  if (keyCode === LEFT_ARROW) {
    moveCapsule(-1, 0);
  } else if (keyCode === RIGHT_ARROW) {
    moveCapsule(1, 0);
  } else if (keyCode === DOWN_ARROW) {
    fallSpeed = FALL_SPEED_FAST;
  } else if (key === 'z' || key === 'Z') {
    rotateCapsule(true); // Clockwise
  } else if (key === 'x' || key === 'X') {
    rotateCapsule(false); // Counter-clockwise
  }
}

function keyReleased() {
  if (keyCode === DOWN_ARROW) {
    fallSpeed = FALL_SPEED_NORMAL;
  }
}

// --------------------------------
// --- CORE GAME LOGIC & STATE  ---
// --------------------------------

function generateRandomCapsule() {
    return {
        x: 3,
        y: 0,
        parts: [
            { x: 0, y: 0, color: floor(random(1, 4)) },
            { x: 1, y: 0, color: floor(random(1, 4)) }
        ],
        orientation: 0, // 0: horizontal, 1: vertical
    };
}

function initializeGrid() {
  grid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
  let virusesPlaced = 0;
  while (virusesPlaced < VIRUS_COUNT) {
    const x = floor(random(BOARD_WIDTH));
    const y = floor(random(BOARD_HEIGHT / 2, BOARD_HEIGHT));
    if (grid[y][x] === 0) {
      const virusColor = floor(random(1, 4));
      grid[y][x] = virusColor + 10;
      virusesPlaced++;
    }
  }
}

function spawnNewCapsule() {
  if (grid[0][3] !== 0 || grid[0][4] !== 0) {
      gameState = 'GAME_OVER';
      capsule = null;
      return;
  }
  capsule = nextCapsule;
  nextCapsule = generateRandomCapsule();
}

function handleFalling() {
  if (!capsule) return;
  fallCounter++;
  if (fallCounter >= fallSpeed) {
    fallCounter = 0;
    if (isValidPosition(capsule.x, capsule.y + 1, capsule.parts, grid)) {
        capsule.y++;
    } else {
        lockCapsule();
    }
  }
}

function lockCapsule() {
    for (const part of capsule.parts) {
        const gridX = capsule.x + part.x;
        const gridY = capsule.y + part.y;
        if (gridY >= 0 && gridY < BOARD_HEIGHT && gridX >= 0 && gridX < BOARD_WIDTH) {
            grid[gridY][gridX] = part.color;
        }
    }
    capsule = null;
    sounds.land.play();

    let chain = 0;
    while (true) {
        const toClear = checkMatches(grid);
        if (toClear.size > 0) {
            chain++;
            score += toClear.size * 10 * chain;
            sounds.clear.play();
            for (const cell of toClear) {
                const [x, y] = cell.split(',').map(Number);
                grid[y][x] = 0;
            }
            applyGravity(grid);
        } else {
            break;
        }
    }
    
    if (getVirusCount() === 0) {
        gameState = 'CLEAR';
        return;
    }
    
    spawnNewCapsule();
}

function moveCapsule(xDir, yDir) {
  if (!capsule) return;
  const newX = capsule.x + xDir;
  const newY = capsule.y + yDir;
  if (isValidPosition(newX, newY, capsule.parts, grid)) {
    capsule.x = newX;
    capsule.y = newY;
    sounds.move.play();
  }
}

function rotateCapsule(clockwise) {
  if (!capsule) return;

  // Toggle orientation between 0 (horizontal) and 1 (vertical)
  const newOrientation = (capsule.orientation + 1) % 2;
  let newParts = JSON.parse(JSON.stringify(capsule.parts));

  // Swap colors of the two parts
  const tempColor = newParts[0].color;
  newParts[0].color = newParts[1].color;
  newParts[1].color = tempColor;

  // Adjust relative positions based on new orientation
  if (newOrientation === 1) { // to Vertical
      newParts[1].x = 0;
      newParts[1].y = 1;
  } else { // to Horizontal
      newParts[1].x = 1;
      newParts[1].y = 0;
  }

  if (isValidPosition(capsule.x, capsule.y, newParts, grid)) {
      capsule.parts = newParts;
      capsule.orientation = newOrientation;
      sounds.rotate.play();
  } 
}

function isValidPosition(gridX, gridY, parts, targetGrid) {
    for (const part of parts) {
        const finalX = gridX + part.x;
        const finalY = gridY + part.y;
        if (finalX < 0 || finalX >= BOARD_WIDTH || finalY < 0 || finalY >= BOARD_HEIGHT) {
            return false;
        }
        // Check against the provided targetGrid
        if (targetGrid[finalY][finalX] !== 0) {
            return false;
        }
    }
    return true;
}

function getVirusCount() {
    let count = 0;
    for (let y = 0; y < BOARD_HEIGHT; y++) {
        for (let x = 0; x < BOARD_WIDTH; x++) {
            if (grid[y][x] > 10) count++;
        }
    }
    return count;
}

function applyGravity(targetGrid) {
    let moved;
    do {
        moved = false;
        for (let y = BOARD_HEIGHT - 2; y >= 0; y--) {
            for (let x = 0; x < BOARD_WIDTH; x++) {
                const cell = targetGrid[y][x];
                if (cell > 0 && cell < 10 && targetGrid[y + 1][x] === 0) {
                    targetGrid[y + 1][x] = cell;
                    targetGrid[y][x] = 0;
                    moved = true;
                }
            }
        }
    } while (moved);
}

function checkMatches(targetGrid) {
    const toClear = new Set();

    // --- Horizontal Check (Robust) ---
    for (let y = 0; y < BOARD_HEIGHT; y++) {
        let matchCount = 0;
        let currentColor = -1;
        for (let x = 0; x <= BOARD_WIDTH; x++) { // Iterate one past the edge
            const cellColor = (x < BOARD_WIDTH) ? targetGrid[y][x] % 10 : -1; // Sentinel
            
            if (cellColor !== 0 && cellColor === currentColor) {
                matchCount++;
            } else {
                if (matchCount >= 4) {
                    for (let i = 1; i <= matchCount; i++) {
                        toClear.add(`${x - i},${y}`);
                    }
                }
                matchCount = 1;
                currentColor = cellColor;
            }
        }
    }

    // --- Vertical Check (Robust) ---
    for (let x = 0; x < BOARD_WIDTH; x++) {
        let matchCount = 0;
        let currentColor = -1;
        for (let y = 0; y <= BOARD_HEIGHT; y++) { // Iterate one past the edge
            const cellColor = (y < BOARD_HEIGHT) ? targetGrid[y][x] % 10 : -1; // Sentinel

            if (cellColor !== 0 && cellColor === currentColor) {
                matchCount++;
            } else {
                if (matchCount >= 4) {
                    for (let i = 1; i <= matchCount; i++) {
                        toClear.add(`${x},${y - i}`);
                    }
                }
                matchCount = 1;
                currentColor = cellColor;
            }
        }
    }
    return toClear;
}


// -----------------------
// --- DRAW FUNCTIONS  ---
// -----------------------

function drawGameBoard() {
  stroke(200);
  strokeWeight(1);
  noFill();
  for (let i = 0; i < BOARD_WIDTH; i++) {
    for (let j = 0; j < BOARD_HEIGHT; j++) {
      rect(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE);
    }
  }
  stroke(200);
  strokeWeight(2);
  line(BOARD_WIDTH * CELL_SIZE, 0, BOARD_WIDTH * CELL_SIZE, CANVAS_HEIGHT);
}

function drawGridContent() {
    for (let y = 0; y < BOARD_HEIGHT; y++) {
        for (let x = 0; x < BOARD_WIDTH; x++) {
            const colorCode = grid[y][x];
            if (colorCode === 0) continue;

            const color = PILL_COLORS[colorCode % 10];
            fill(color);
            stroke(51);
            strokeWeight(2);

            const centerX = x * CELL_SIZE + CELL_SIZE / 2;
            const centerY = y * CELL_SIZE + CELL_SIZE / 2;

            if (colorCode > 10) {
                image(images.viruses[colorCode % 10], x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            } else {
                rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }
    }
}

function drawCapsule() {
  if (!capsule) return;
  for (const part of capsule.parts) {
    const x = (capsule.x + part.x) * CELL_SIZE;
    const y = (capsule.y + part.y) * CELL_SIZE;
    fill(PILL_COLORS[part.color]);
    stroke(51);
    strokeWeight(2);
    rect(x, y, CELL_SIZE, CELL_SIZE);
  }
}

function drawUI() {
    const uiX = BOARD_PIXEL_WIDTH + 20;
    fill(255);
    textSize(24);
    textAlign(LEFT, TOP);

    text("SCORE", uiX, 20);
    textSize(32);
    text(score, uiX, 50);

    text("VIRUS", uiX, 120);
    textSize(32);
    text(getVirusCount(), uiX, 150);

    text("NEXT", uiX, 220);
    if (nextCapsule) {
        const nextX = uiX + CELL_SIZE;
        const nextY = 260;
        for (const part of nextCapsule.parts) {
            fill(PILL_COLORS[part.color]);
            stroke(51);
            strokeWeight(2);
            rect(nextX + part.x * CELL_SIZE, nextY + part.y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
    }
}

function drawOverlay(msg, bgColor) {
    const finalBgColor = bgColor === undefined ? color(0, 0, 0, 150) : bgColor;
    fill(finalBgColor);
    rect(0, 0, BOARD_PIXEL_WIDTH, CANVAS_HEIGHT);
    
    fill(255);
    textSize(40);
    textAlign(CENTER, CENTER);
    text(msg, BOARD_PIXEL_WIDTH / 2, CANVAS_HEIGHT / 2);
}
