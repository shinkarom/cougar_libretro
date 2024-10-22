const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const tilesDir = 'tiles';

// Ensure the tiles directory exists
if (!fs.existsSync(tilesDir)) {
    fs.mkdirSync(tilesDir, { recursive: true });
}

function findMissingNumbers() {
    const missingNumbers = [];
    for (let i = 0; i <= 0x7; i++) {
        const hexNumber = i.toString(16).padStart(1, '0');
        const fileName = `CHR_${hexNumber}.png`;
        const filePath = path.join(tilesDir, fileName);

        if (!fs.existsSync(filePath)) {
            missingNumbers.push(fileName);
        }
    }
    return missingNumbers;
}

async function createImage(filePath) {
    await sharp({
        create: {
            width: 128,
            height: 128,
            channels: 4, // 4 channels for RGBA
            background: { r: 0, g: 0, b: 0, alpha: 0 } // Transparent background
        }
    }).toFile(filePath);
    console.log('Blank PNG file created:', filePath);
}

async function createMissingImages() {
    const missingFiles = findMissingNumbers();

    if (missingFiles.length > 0) {
        for (const fileName of missingFiles) {
            const filePath = path.join(tilesDir, fileName);
            await createImage(filePath);
        }
    } else {
        console.log('All numbers are filled, no action taken.');
    }
}

// Start the process to create missing images
createMissingImages().catch(err => console.error('Error creating images:', err));
