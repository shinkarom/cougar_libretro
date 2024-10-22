const fs = require('fs');
const path = require('path');
const { program } = require('commander');
const { parseString } = require('xml2js');

const inputDirectoryPath = './maps'; // Replace with your input folder path
const outputDirectoryPath = './src'; // Use the provided output directory or default to './output'

// Create the output directory if it doesn't exist
if (!fs.existsSync(outputDirectoryPath)) {
  fs.mkdirSync(outputDirectoryPath);
}

fs.readdir(inputDirectoryPath, (err, files) => {
  if (err) {
    console.error('Error reading directory:', err);
    return;
  }

  // Filter for files with a .tmx extension
  const tmxFiles = files.filter(file => path.extname(file) === '.tmx');

  tmxFiles.forEach(async tmxFile => {
    const filePath = path.join(inputDirectoryPath, tmxFile);

    // Read the TMX file
    const tmxXml = fs.readFileSync(filePath, 'utf-8');

    // Parse the TMX XML manually
    parseString(tmxXml, (err, result) => {
      if (err) {
        console.error(`Error parsing TMX file: ${tmxFile}`, err);
        return;
      }

      const map = result.map;

      // Find the layer with ID 1
      const layer = map.layer.find(layer => layer.$.id === '1');

      if (layer) {
        // Extract width and height of the layer
        const width = parseInt(layer.$.width, 10);
        const height = parseInt(layer.$.height, 10);

        // Process and format the data
        const formattedData = layer.data[0]._.split(',').map(value => {
          // Parse the value as an integer
          const intValue = parseInt(value, 10);

          // Extract the first 16 bits
          const bits = intValue & 0xFFFF;

          // Decrease the value by 1 if it's greater than 0
          const newValue = bits > 0 ? bits - 1 : bits;

          return newValue;
        });

        // Create an object with the layer's width, height, and formatted data
        const jsonData = {
          width,
          height,
          data: formattedData,
        };

        // Write the JSON data to an output file
        const outputFileName = path.join("src", `${tmxFile}.json`);
        fs.writeFileSync(outputFileName, JSON.stringify(jsonData, null, 0));

        console.log(`Processed TMX file: ${tmxFile}`);
      } else {
        console.log(`Layer with ID 1 not found in TMX file: ${tmxFile}`);
      }
    });
  });
});