// Global Setup
// Mock p5.js functions for testing environment
let originalCapsule, originalNextCapsule, originalGrid, originalGameState, originalScore;
let mockSounds = { move: { play: () => {} }, rotate: { play: () => {} }, land: { play: () => {} }, clear: { play: () => {} } };

QUnit.hooks.beforeEach(function() {
  window.floor = Math.floor; // floorを最初にモック
  // Mock p5.js random function for predictable results
  let randomValues = [1.5, 2.5, 3.5, 1.5]; // -> floor() -> 1, 2, 3, 1
  let randomCallIndex = 0;
  window.random = function(min, max) {
    if (max === undefined) {
      max = min;
      min = 0;
    }
    // Return a predictable sequence for testing generateRandomCapsule
    if (QUnit.config.current.testName === 'generateRandomCapsule: generates valid capsule') {
      return 1.5; // Always results in color 2
    }
    const result = randomValues[randomCallIndex % randomValues.length];
    randomCallIndex++;
    return result;
  };

  // Save original global state
  originalCapsule = capsule;
  originalNextCapsule = nextCapsule;
  originalGrid = grid;
  originalGameState = gameState;
  originalScore = score;

  // Reset global state for each test
  capsule = null;
  nextCapsule = null;
  grid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
  gameState = 'PLAYING';
  score = 0;
  sounds = mockSounds; // Use mock sounds

  window.keyCode = 0; // Mock keyCode
  window.ENTER = 13; // Mock ENTER key code
  window.SPACE = 32; // Mock SPACE key code
});

QUnit.hooks.afterEach(function() {
  // Restore original global state
  capsule = originalCapsule;
  nextCapsule = originalNextCapsule;
  grid = originalGrid;
  gameState = originalGameState;
  score = originalScore;
  sounds = {}; // Clear mock sounds
  window.random = undefined; // Clear mock
  window.floor = undefined; // Clear mock
});

// 動作確認用
// QUnit.module('Sanity Check', function() {
//   QUnit.test('True is true', function(assert) {
//     assert.ok(true, 'This test should always pass');
//   });
// });


QUnit.module('Core Game Mechanics', function() {
  /**
   * Tests the most basic horizontal clear scenario.
   * It sets up a line of 4 identical capsule parts and asserts that
   * the checkMatches function correctly identifies all 4 for clearing.
   */
  QUnit.test('checkMatches: horizontal clear of 4', function(assert) {
    const testGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    testGrid[15][0] = 1;
    testGrid[15][1] = 1;
    testGrid[15][2] = 1;
    testGrid[15][3] = 1;

    const toClear = checkMatches(testGrid);
    assert.equal(toClear.size, 4, 'Should find 4 cells to clear');
    assert.ok(toClear.has('0,15') && toClear.has('3,15'), 'Correctly identifies cells to clear');
  });

  /**
   * Tests a vertical clear that includes a mix of capsule parts and viruses.
   * This ensures the color matching logic (using modulo 10) works correctly
   * regardless of the block type.
   */
  QUnit.test('checkMatches: vertical clear of 4 with viruses', function(assert) {
    const testGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    testGrid[12][0] = 13; // Virus (color 3)
    testGrid[13][0] = 3;  // Capsule (color 3)
    testGrid[14][0] = 13; // Virus (color 3)
    testGrid[15][0] = 3;  // Capsule (color 3)

    const toClear = checkMatches(testGrid);
    assert.equal(toClear.size, 4, 'Should find 4 cells to clear');
    assert.ok(toClear.has('0,12') && toClear.has('0,15'), 'Correctly identifies cells to clear in a mixed vertical line');
  });

  /**
   * Tests the basic failure case where not enough blocks are in a row.
   * This ensures the game doesn't clear blocks incorrectly.
   */
  QUnit.test('checkMatches: no clear', function(assert) {
    const testGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    testGrid[15][0] = 1;
    testGrid[15][1] = 1;
    testGrid[15][2] = 1; // Only 3 in a row

    const toClear = checkMatches(testGrid);
    assert.equal(toClear.size, 0, 'Should find no cells to clear');
  });

  /**
   * Tests the basic gravity mechanic on a single, unsupported capsule part.
   * It verifies that a floating piece falls to the lowest possible empty space.
   */
  QUnit.test('applyGravity: single capsule part falls', function(assert) {
    const testGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    testGrid[10][3] = 1; // A floating red capsule part

    applyGravity(testGrid);

    const expectedGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    expectedGrid[15][3] = 1; // It should fall to the bottom

    assert.deepEqual(testGrid, expectedGrid, 'Single floating piece should fall to the bottom');
  });

  /**
   * Tests a specific game rule: viruses are static and should not fall,
   * even if the space below them is empty.
   */
  QUnit.test('applyGravity: viruses do not fall', function(assert) {
    const testGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    testGrid[10][3] = 11; // A floating red virus
    const originalGrid = JSON.parse(JSON.stringify(testGrid));

    applyGravity(testGrid);

    assert.deepEqual(testGrid, originalGrid, 'Viruses should not be affected by gravity');
  });

  /**
   * This is an integration test for the core game loop.
   * It verifies that clearing a line correctly triggers gravity, causing
   * pieces above the cleared line to fall into the empty space.
   */
  QUnit.test('Game Loop: Full clear and gravity chain', function(assert) {
    const testGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    // Setup: A piece '1' is above a line of four '2's
    testGrid[14][1] = 1;
    testGrid[15] = [2, 2, 2, 2, 0, 0, 0, 0];

    // 1. Simulate finding matches
    const toClear = checkMatches(testGrid);
    assert.equal(toClear.size, 4, "Finds the horizontal line of four 2s");

    // 2. Simulate clearing the grid
    for (const cell of toClear) {
        const [x, y] = cell.split(',').map(Number);
        testGrid[y][x] = 0;
    }

    // 3. Simulate gravity
    applyGravity(testGrid);

    // 4. Assert the final state
    const expectedGrid = Array(BOARD_HEIGHT).fill(0).map(() => Array(BOARD_WIDTH).fill(0));
    expectedGrid[15][1] = 1; // The '1' should have fallen into the cleared space

    assert.deepEqual(testGrid, expectedGrid, 'Piece should fall correctly after a line is cleared');
  });

  /**
   * Tests isValidPosition for boundary checks.
   * Ensures a capsule cannot move outside the board boundaries.
   */
  QUnit.test('isValidPosition: boundary checks', function(assert) {
    capsule = generateRandomCapsule(); // Need a global capsule for isValidPosition
    capsule.parts = [{x:0,y:0,color:1}, {x:1,y:0,color:2}]; // Horizontal capsule

    // Test moving left out of bounds
    assert.notOk(isValidPosition(-1, 0, capsule.parts, grid), 'Cannot move left out of bounds');
    // Test moving right out of bounds
    assert.notOk(isValidPosition(BOARD_WIDTH - 1, 0, capsule.parts, grid), 'Cannot move right out of bounds');
    // Test moving down out of bounds
    assert.notOk(isValidPosition(0, BOARD_HEIGHT, capsule.parts, grid), 'Cannot move down out of bounds');
    // Test valid position
    assert.ok(isValidPosition(0, 0, capsule.parts, grid), 'Can move to a valid position');
  });

  /**
   * Tests isValidPosition for collision with existing blocks in the grid.
   * Ensures a capsule cannot move into an occupied cell.
   */
  QUnit.test('isValidPosition: collision with existing blocks', function(assert) {
    capsule = generateRandomCapsule(); // Need a global capsule
    capsule.parts = [{x:0,y:0,color:1}, {x:1,y:0,color:2}];
    grid[0][0] = 1; // Occupy a cell

    assert.notOk(isValidPosition(0, 0, capsule.parts, grid), 'Cannot move into an occupied cell');
    grid[0][0] = 0; // Clear for next check
    grid[0][1] = 1; // Occupy the second part's position
    assert.notOk(isValidPosition(0, 0, capsule.parts, grid), 'Cannot move into an occupied cell (second part)');
  });
});


QUnit.module('Capsule Management', function() {
  /**
   * Tests that generateRandomCapsule creates a capsule object with the expected structure
   * and that its colors are within the valid range (1-3).
   */
  QUnit.test('generateRandomCapsule: generates valid capsule', function(assert) {
    const capsule = generateRandomCapsule();
    assert.ok(capsule.x !== undefined && capsule.y !== undefined, 'Capsule has x and y properties');
    assert.ok(Array.isArray(capsule.parts) && capsule.parts.length === 2, 'Capsule has two parts');
    assert.ok(capsule.parts[0].color >= 1 && capsule.parts[0].color <= 3, 'Part 0 color is valid');
    assert.ok(capsule.parts[1].color >= 1 && capsule.parts[1].color <= 3, 'Part 1 color is valid');
    assert.equal(capsule.orientation, 0, 'Capsule starts with orientation 0');
  });

  /**
   * Tests spawnNewCapsule function.
   * Ensures it correctly transfers nextCapsule to capsule and generates a new nextCapsule.
   * Also tests the game over condition.
   */
  QUnit.test('spawnNewCapsule: transfers and generates new capsule', function(assert) {
    const initialNextCapsule = generateRandomCapsule();
    nextCapsule = initialNextCapsule;
    capsule = null; // Ensure capsule is initially null

    spawnNewCapsule();

    assert.deepEqual(capsule, initialNextCapsule, 'nextCapsule should become current capsule');
    assert.notDeepEqual(nextCapsule, initialNextCapsule, 'A new nextCapsule should be generated');
    assert.equal(gameState, 'PLAYING', 'Game state should remain PLAYING');
  });

  QUnit.test('spawnNewCapsule: triggers GAME_OVER if spawn area is blocked', function(assert) {
    grid[0][3] = 1; // Block spawn area
    grid[0][4] = 2; // Block spawn area
    nextCapsule = generateRandomCapsule(); // Needs to be defined
    capsule = null;

    spawnNewCapsule();

    assert.equal(gameState, 'GAME_OVER', 'Game state should be GAME_OVER');
    assert.equal(capsule, null, 'Current capsule should be null');
  });

  /**
   * Tests moveCapsule function.
   * Ensures capsule moves correctly within bounds and is blocked by walls/other blocks.
   */
  QUnit.test('moveCapsule: moves capsule correctly', function(assert) {
    capsule = { x: 3, y: 0, parts: [{x:0,y:0,color:1}, {x:1,y:0,color:2}], orientation: 0 };
    const initialX = capsule.x;

    moveCapsule(1, 0); // Move right
    assert.equal(capsule.x, initialX + 1, 'Capsule should move right');

    moveCapsule(-1, 0); // Move left
    assert.equal(capsule.x, initialX, 'Capsule should move left');

    // Test collision with wall
    capsule.x = BOARD_WIDTH - 2; // Last valid position for a horizontal capsule.
    moveCapsule(1, 0); // Try to move right
    assert.equal(capsule.x, BOARD_WIDTH - 2, 'Capsule should not move right out of bounds');

    // Test collision with block
    grid[0][2] = 1; // Place block to the left
    capsule.x = 3;
    moveCapsule(-1, 0); // Try to move left into block
    assert.equal(capsule.x, 3, 'Capsule should not move into existing block');
  });

    /**
     * Tests rotateCapsule function.
     * Ensures capsule rotates through 4 states and colors swap correctly.
     */
    QUnit.test('rotateCapsule: rotates capsule through 4 states', function(assert) {
      capsule = { x: 3, y: 1, parts: [{x:0,y:0,color:1}, {x:1,y:0,color:2}], orientation: 0 };
      const initialColor0 = capsule.parts[0].color;
      const initialColor1 = capsule.parts[1].color;
  
      // State 0: Right
      assert.equal(capsule.orientation, 0, 'Starts at orientation 0 (Right)');
      assert.deepEqual(capsule.parts[1], {x:1,y:0,color:initialColor1}, 'Part 1 is to the right');
  
      // Rotate 1 (0 -> 1: Down)
      rotateCapsule(true);
      assert.equal(capsule.orientation, 1, 'Orientation should be 1 (Down)');
      assert.deepEqual(capsule.parts[0], {x:0,y:0,color:initialColor0}, 'Part 0 color should remain initial Part 0 color');
      assert.deepEqual(capsule.parts[1], {x:0,y:1,color:initialColor1}, 'Part 1 color should remain initial Part 1 color and position below');
  
      // Rotate 2 (1 -> 2: Left)
      rotateCapsule(true);
      assert.equal(capsule.orientation, 2, 'Orientation should be 2 (Left)');
      assert.deepEqual(capsule.parts[0], {x:0,y:0,color:initialColor0}, 'Part 0 color should remain initial Part 0 color');
      assert.deepEqual(capsule.parts[1], {x:-1,y:0,color:initialColor1}, 'Part 1 color should remain initial Part 1 color and position left');
  
      // Rotate 3 (2 -> 3: Up)
      rotateCapsule(true);
      assert.equal(capsule.orientation, 3, 'Orientation should be 3 (Up)');
      assert.deepEqual(capsule.parts[0], {x:0,y:0,color:initialColor0}, 'Part 0 color should remain initial Part 0 color');
      assert.deepEqual(capsule.parts[1], {x:0,y:-1,color:initialColor1}, 'Part 1 color should remain initial Part 1 color and position above');
  
      // Rotate 4 (3 -> 0: Right)
      rotateCapsule(true);
      assert.equal(capsule.orientation, 0, 'Orientation should be 0 (Right) again');
      assert.deepEqual(capsule.parts[0], {x:0,y:0,color:initialColor0}, 'Part 0 color should remain initial Part 0 color');
      assert.deepEqual(capsule.parts[1], {x:1,y:0,color:initialColor1}, 'Part 1 color should remain initial Part 1 color and position right again');
  
      // Test collision during rotation
      capsule.x = 1;
      capsule.y = 1;
      capsule.orientation = 0; // Right
      capsule.parts = [{x:0,y:0,color:1}, {x:1,y:0,color:2}];
      grid[2][1] = 3; // Block the spot for state 1 (Down)
      rotateCapsule(true);
      assert.equal(capsule.orientation, 0, 'Capsule should not rotate if blocked');
    });});


QUnit.module('Game State & Scoring', function() {
  /**
   * Tests getVirusCount function.
   * Ensures it accurately counts only viruses (colorCode > 10).
   */
  QUnit.test('getVirusCount: counts viruses correctly', function(assert) {
    grid[0][0] = 11; // Virus
    grid[0][1] = 1;  // Capsule part
    grid[1][0] = 12; // Virus
    grid[1][1] = 0;  // Empty

    assert.equal(getVirusCount(), 2, 'Should count only virus blocks');
  });
});
