const Module = new WebAssembly.Module(await fetch('demo.wasm').then(response => response.arrayBuffer()));
const instance = new WebAssembly.Instance(Module, {
  env: {
    squer_import_function: () => console.log("Squer approves")
  }
});

const triggerSquer = instance.exports.triggerSquer;

triggerSquer();