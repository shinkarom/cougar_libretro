const fs = require('fs');
const archiver = require('archiver');

const sourceFolder = './'; // Path to your project folder
const outputFileName = process.argv[2] || "project";
const outputFilePath = "./output/"+outputFileName+'.zip';

const archive = archiver('zip', { zlib: { level: 9 } }); // Create a zip archive

// Create an output stream to save the zip file
const output = fs.createWriteStream(`${outputFilePath}`);

// Pipe the output stream to the archive
archive.pipe(output);

// Exclude "node_modules" and "output" folders from the archive
archive.glob('**/*', {
  cwd: sourceFolder,
  ignore: ['node_modules/**', 'output/**'],
});

// Listen for the 'close' event to know when the archive has been created
archive.on('close', () => {
  console.log('Archive created successfully.');
});

// Finalize the archive (this will trigger the 'close' event)
archive.finalize();