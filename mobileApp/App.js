/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow
 */

import React from 'react';
import { Button, Image, SafeAreaView, ScrollView, StatusBar, Text, View } from 'react-native';
import vision from '@react-native-firebase/ml-vision';
import ImagePicker from 'react-native-image-picker';
import {init, cloudVisionProcessImage, visionProcessImage, detect, VisionImage} from './ml-vision'

const options = {
  title: 'Select photo',
  customButtons: [],
  storageOptions: {
    skipBackup: true,
    path: 'images',
  },
};

class App extends React.PureComponent {
  state = {
    image: null,
    deviceResult: [],
    cloudResult: [],
  };

  async componentDidMount(): void {
    await init('vision.tflite', false)
    // await init('', true)
  }

  getPhoto = () => {
    ImagePicker.showImagePicker(options, (response) => {
      if (response.didCancel) {
        console.log('User cancelled image picker');
      } else if (response.error) {
        console.log('ImagePicker Error: ', response.error);
      } else if (response.customButton) {
        console.log('User tapped custom button: ', response.customButton);
      } else {
        this.setState({
          image: { uri: response.uri },
        }, async () => {
          const visionImage = new VisionImage(response.uri, false);

          // const result = await detect(visionImage);
          const cloudResult = await cloudVisionProcessImage(visionImage);
          const deviceResult = await visionProcessImage(visionImage);
          this.setState({
            cloudResult,
            deviceResult,
          });
        });
      }
    });
  };

  render(): React.ReactNode {
    const { image, cloudResult, deviceResult } = this.state;
    return (
      <>
        <StatusBar barStyle="dark-content"/>
        <SafeAreaView style={{ flex: 1, paddingHorizontal: 16 }}>
          <Button
            style={{ height: 56, width: '100%' }}
            title={'Select or take photo'}
            onPress={this.getPhoto}
          />
          <Image
            resizeMode={'contain'}
            source={image}
            style={{ width: '100%', height: 200, borderRadius: 24, marginTop: 24 }}
          />
          <View
            style={{
              flexDirection: 'row',
              justifyContent: 'space-between',
              alignItems: 'flex-start',
              width: '100%',
            }}>
            <ScrollView
              bounces={false}
              style={{ flex: 1 }}
              contentContainerStyle={{
                flexDirection: 'column',
                alignItems: 'center',
                paddingVertical: 24,
                paddingHorizontal: 16,
              }}
            >
              {deviceResult && deviceResult.length > 0 ?
                <Text style={{ fontSize: 24, marginBottom: 16 }}>
                  {'Device'}
                </Text>
                : null
              }
              {(deviceResult || []).map((resultItem, resultIndex) => {
                return (
                  <View
                    key={'device' + resultIndex}
                    style={{
                      width: '100%',
                      flexDirection: 'column',
                      alignItems: 'flex-start',
                      marginTop: resultIndex === 0 ? 0 : 12,
                    }}
                  >
                    <Text style={{ fontSize: 18 }}>
                      {resultItem.text}
                    </Text>
                    <Text style={{ fontSize: 14, marginTop: 4 }}>
                      {resultItem.confidence}
                    </Text>
                  </View>
                );
              })}
            </ScrollView>
            <ScrollView
              bounces={false}
              style={{ flex: 1 }}
              contentContainerStyle={{
                flexDirection: 'column',
                alignItems: 'center',
                paddingVertical: 24,
                paddingHorizontal: 16,
              }}
            >
              {cloudResult && cloudResult.length > 0 ?
                <Text style={{ fontSize: 24, marginBottom: 16 }}>
                  {'Cloud'}
                </Text>
                : null
              }
              {(cloudResult || []).map((resultItem, resultIndex) => {
                return (
                  <View
                    key={'cloud' + resultIndex}
                    style={{
                      width: '100%',
                      flexDirection: 'column',
                      alignItems: 'flex-start',
                      marginTop: resultIndex === 0 ? 0 : 12,
                    }}
                  >
                    <Text style={{ fontSize: 18 }}>
                      {resultItem.text}
                    </Text>
                    <Text style={{ fontSize: 14, marginTop: 4 }}>
                      {resultItem.confidence}
                    </Text>
                  </View>
                );
              })}
            </ScrollView>
          </View>
        </SafeAreaView>
      </>
    );
  }
}

export default App;
