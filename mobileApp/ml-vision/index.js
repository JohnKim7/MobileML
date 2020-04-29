import vision from '@react-native-firebase/ml-vision';

let visionModule = null;
let useCloud = false;

export class VisionImage {
  constructor(content, isBase64) {
    this.content = content;
    this.isBase64 = isBase64;
  }

  getContent = () => {
    if (this.isBase64) {
      return null;
    } else {
      return this.content;
    }
  };
}

export const init = async (filePath, isCloud) => {
  visionModule = await vision();
  useCloud = isCloud;
  return true;
};

export const detect = async (visionImage) => {
  if (useCloud) {
    return await cloudVisionProcessImage(visionImage)
  } else {
    return await visionProcessImage(visionImage)
  }
};

export const visionProcessImage = async (visionImage) => {
  if (visionModule !== null) {
    return await visionModule.imageLabelerProcessImage(visionImage.content, {
      confidenceThreshold: 0.1,
    });
  } else {
    throw Error('Vision not initialized');
  }
};


export const cloudVisionProcessImage = async (visionImage) => {
  if (visionModule !== null) {
    return await visionModule.cloudImageLabelerProcessImage(visionImage.content, {
      confidenceThreshold: 0.1,
    });
  } else {
    throw Error('Vision not initialized');
  }
};
