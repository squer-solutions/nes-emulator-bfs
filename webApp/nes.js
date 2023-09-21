import Module from "./simple-nes.js";

const width = 256;
const height = 240;
const pixelWidth = 2;
const pixelSpacing = 0;

let emulator;
let screen;

const canvas = document.getElementById("shibe");
canvas.width = 1000;
canvas.height = 1000;
const ctx = canvas.getContext("2d");

async function loadEmulator() {
  const wasm = await Module();
  emulator = new wasm.SimpleNesWasmContext(width, height, "mario-bros.nes");
  const result = emulator.run();
  console.log("Emulator run result:", result);

  if (result !== "Success") {
    throw "failed to start emulator"
  }

  screen = emulator.rawScreen();
}

loadEmulator().then(_ => {
  console.log("Loaded emulator");
  renderLoop();

  const mapping = (key) => {
    switch (key) {
      case 'Spacebar':
      case ' ':
        return 'space';
      case 'Shift':
        return 'rshift';
      default:
        return key;
    }
  };

  document.addEventListener('keydown', (event) => {
    if (event.key === ' ' || event.key === 'Spacebar') {
      event.preventDefault();
    }

    emulator.press(mapping(event.key));
  });

  document.addEventListener('keyup', function(event) {
    emulator.release(mapping(event.key));
  });
}).catch(err => console.error(err));


function renderLoop() {
  emulator.tick();
  draw();
  requestAnimationFrame(renderLoop);
}

const draw = () => {
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const i = (x + width * y) * 3;

      ctx.fillStyle = `rgb(
                ${screen[i]},
                ${screen[i + 1]},
                ${screen[i + 2]}
            )`;

      ctx.fillRect(
        x * (pixelWidth + pixelSpacing),
        y * (pixelWidth + pixelSpacing),
        pixelWidth,
        pixelWidth
      );
    }
  }
}
