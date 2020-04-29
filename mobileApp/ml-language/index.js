import language from '@react-native-firebase/ml-natural-language';

let languageModule = null;
let useCloud = false;

export const init = async (filePath, isCloud) => {
  languageModule = await language();
  useCloud = isCloud;
  return true;
};

export const detect = async (languageText) => {
  if (useCloud) {
    return await cloudIdentifyLanguage(languageText)
  } else {
    return await identifyLanguage(languageText)
  }
};

export const identifyLanguage = async (languageText) => {
  if (languageModule !== null) {
    return await languageModule.identifyLanguage(languageText, {
      confidenceThreshold: 0.1,
    });
  } else {
    throw Error('language not initialized');
  }
};

export const cloudIdentifyLanguage = async (languageText) => {
  if (languageModule !== null) {
    return await languageModule.cloudIdentifyLanguage(languageText, {
      confidenceThreshold: 0.1,
    });
  } else {
    throw Error('language not initialized');
  }
};
