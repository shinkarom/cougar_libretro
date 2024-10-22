const { spawn } = require('child_process');
const path = require('path');

projectName = require("../package.json").name
// Get the file path from the command line arguments
const fileToRun = "output\\"+projectName+".cart";
console.log(fileToRun)
// Define the path to the RetroArch executable
const retroArchPath = 'C:\\Program Files\\RetroArch\\retroarch.exe'; // Windows

// Define the path to the core
const corePath = '..\\cougar_libretro\\bin\\cougar_libretro.dll'; // Update with your core path

// Spawn the RetroArch process with the necessary arguments
const retroArchProcess = spawn(retroArchPath, ['-L', corePath, fileToRun]);

// Handle standard output
retroArchProcess.stdout.on('data', (data) => {
    console.log(`stdout: ${data}`);
});

// Handle standard error
retroArchProcess.stderr.on('data', (data) => {
    console.error(`stderr: ${data}`);
});

// Handle process exit
retroArchProcess.on('close', (code) => {
    console.log(`RetroArch exited with code ${code}`);
});
