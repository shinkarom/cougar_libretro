const fs = require('fs');
const path = require('path');

function findAvailableNumber() {
  for (let i = 0; i <= 0xFF; i++) {
    const hexNumber = i.toString(16).padStart(1, '0');
    const fileName = `CHR_${hexNumber}.png`;
    const filePath = path.join('tiles', fileName);

    if (!fs.existsSync(filePath)) {
      return fileName;
    }
  }

  return null; // All numbers are filled
}

const availableFileName = findAvailableNumber();

if (availableFileName) {
  // Create a blank 128x128 PNG file here
  // You can use libraries like 'pngjs' or 'sharp' to create the PNG file
  // For example, using 'sharp':
  const sharp = require('sharp');

  sharp({
    create: {
      width: 128,
      height: 128,
      channels: 4, // 4 channels for RGBA
      background: { r: 0, g: 0, b: 0, alpha: 0 } // White background with transparency
    }
  })
  .toFile(path.join('tiles', availableFileName), (err, info) => {
    if (err) {
      console.error('Error creating the PNG file:', err);
    } else {
      console.log('Blank PNG file created:', availableFileName);
    }
  });
} else {
  console.log('All numbers are filled, no action taken.');
}