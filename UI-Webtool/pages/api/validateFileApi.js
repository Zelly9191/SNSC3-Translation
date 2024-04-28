const path = require('path');
const fs = require('fs');

let wasmMemory = new WebAssembly.Memory({initial: 256, maximum: 256});

let wasmTable = new WebAssembly.Table({
    'initial': 1,
    'maximum': 1,
    'element': 'anyfunc'
});

let asmLibraryArg = { 
    "__handle_stack_overflow": ()=>{},
    "emscripten_resize_heap": ()=>{},
    "__lock": ()=>{}, 
    "__unlock": ()=>{},
    "memory": wasmMemory, 
    "table": wasmTable 
};

var info = {
    'env': asmLibraryArg,
    'wasi_snapshot_preview1': asmLibraryArg
};

export default async function handler(req, res) {
    if (req.method !== 'GET') {
        res.status(405).send({ message: 'Only GET requests allowed' })
        return;
    }; 

    const wasmLocation = path.join(process.cwd(), 'public', '/web_assembly/mysimple.wasm');    
    const wasmBuffer = fs.readFileSync(wasmLocation);

    WebAssembly.instantiate(wasmBuffer, info).then(wasmModule => {
        const { add } = wasmModule.instance.exports;
        const arrs = add(2,7);
        console.log(arrs); 
    });

    res.status(200).end()
};
