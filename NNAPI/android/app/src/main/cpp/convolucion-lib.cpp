//
// Created by juanm on 20/10/2018.
//
#include <jni.h>
#include <string>
#include <android/NeuralNetworks.h>
#include <float.h>
#include <android/log.h>
#include <sstream>
#include <iomanip>
#include <fcntl.h>

extern "C"
JNIEXPORT void JNICALL
Java_gisia_martin_com_perceptron_MainActivity_convolucionModel(JNIEnv *env, jobject instance) {

    ANeuralNetworksModel* model = NULL;
    ANeuralNetworksModel_create(&model);

    ANeuralNetworksOperandType input, filter, bias, padding, walkWidht, walkHeight, activation, output;

    input.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    input.scale = 0.f;
    input.zeroPoint = 0;
    input.dimensionCount = 4;
    uint32_t dimsInput[4] = {1,5,5,1}; //batches, height, width, depth_in
    input.dimensions = dimsInput;

    filter.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    filter.scale = 0.f;
    filter.zeroPoint = 0;
    filter.dimensionCount = 4;
    uint32_t dimsFilter[4] = {1,2,2,1}; //depth_out, filter_height, filter_width, depth_in
    filter.dimensions = dimsFilter;

    bias.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    bias.scale = 0.f;
    bias.zeroPoint = 0;
    bias.dimensionCount = 1;
    uint32_t dimsBias[1] = {1}; //depth_out
    bias.dimensions = dimsBias;

    padding.type = ANEURALNETWORKS_INT32;
    padding.scale = 0.f;
    padding.zeroPoint = 0;
    padding.dimensionCount = 0;
    padding.dimensions = nullptr;

    walkWidht.type = ANEURALNETWORKS_INT32;
    walkWidht.scale = 0.f;
    walkWidht.zeroPoint = 0;
    walkWidht.dimensionCount = 0;
    walkWidht.dimensions = nullptr;

    walkHeight.type = ANEURALNETWORKS_INT32;
    walkHeight.scale = 0.f;
    walkHeight.zeroPoint = 0;
    walkHeight.dimensionCount = 0;
    walkHeight.dimensions = nullptr;

    activation.type = ANEURALNETWORKS_INT32;
    activation.scale = 0.f;
    activation.zeroPoint = 0;
    activation.dimensionCount = 0;
    activation.dimensions = nullptr;

    output.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    output.scale = 0.f;
    output.zeroPoint = 0;
    output.dimensionCount = 4;
    uint32_t dimsOutput[4] = {1,4,4,1}; //batches, out_height, out_width, depth_out
    output.dimensions = dimsOutput;






    ANeuralNetworksModel_addOperand(model, &input);  // operand 0
    ANeuralNetworksModel_addOperand(model, &filter);  // operand 1
    ANeuralNetworksModel_addOperand(model, &bias);  // operand 2
    ANeuralNetworksModel_addOperand(model, &padding);  // operand 3
    ANeuralNetworksModel_addOperand(model, &walkWidht); // operand 4
    ANeuralNetworksModel_addOperand(model, &walkHeight);  // operand 5
    ANeuralNetworksModel_addOperand(model, &activation); // operand 6
    ANeuralNetworksModel_addOperand(model, &output); // operand 7




    //Input values
    //float inputValues[9] = {23,34,32,23,43,43,23,12,188};
    //ANeuralNetworksModel_setOperandValue(model, 0, &inputValues, sizeof(inputValues));
    //Filter values
    float filterValues[4]={0,1,1,0};
    ANeuralNetworksModel_setOperandValue(model, 1, &filterValues, sizeof(filterValues));
    //Bias values
    float biasValues[1] = {0};
    ANeuralNetworksModel_setOperandValue(model, 2, &biasValues, sizeof(biasValues));



    //Padding
    int32_t paddingValue = ANEURALNETWORKS_PADDING_VALID;
    ANeuralNetworksModel_setOperandValue(model, 3, &paddingValue, sizeof(paddingValue));
    //walkWidht
    int32_t walkWidhtValue = 1;
    ANeuralNetworksModel_setOperandValue(model, 4, &walkWidhtValue, sizeof(walkWidhtValue));
    //walkHeight
    int32_t walkHeightValue = 1;
    ANeuralNetworksModel_setOperandValue(model, 5, &walkHeightValue, sizeof(walkHeightValue));
    //Funcion activacion
    int32_t noneValue = ANEURALNETWORKS_FUSED_NONE;
    ANeuralNetworksModel_setOperandValue(model, 6, &noneValue, sizeof(noneValue));



    //Operacion de convolucion
    uint32_t addInputIndexes[7] = {0,1,2,3,4,5,6};
    uint32_t addOutputIndexes[1] = {7};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, addInputIndexes, 1, addOutputIndexes);


    // Our model has one input (0) and one output (6).
    uint32_t modelInputIndexes[1] = {0};
    uint32_t modelOutputIndexes[1] = {7};
    ANeuralNetworksModel_identifyInputsAndOutputs(model, 1, modelInputIndexes, 1 ,modelOutputIndexes);


    ANeuralNetworksModel_finish(model);
    // Compile the model.
    ANeuralNetworksCompilation* compilation;
    ANeuralNetworksCompilation_create(model, &compilation);
    // Ask to optimize for low power consumption.
    ANeuralNetworksCompilation_setPreference(compilation, ANEURALNETWORKS_PREFER_LOW_POWER);
    ANeuralNetworksCompilation_finish(compilation);
    // Run the compiled model against a set of inputs.
    ANeuralNetworksExecution* run1 = NULL;
    ANeuralNetworksExecution_create(compilation, &run1);


    float inputValues[1][5][5][1] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    ANeuralNetworksExecution_setInput(run1, 0, NULL, inputValues, sizeof(inputValues));
    // Set the output.
    float myOutput[1][4][4][1];
    ANeuralNetworksExecution_setOutput(run1, 0, NULL, myOutput, sizeof(myOutput));



    // Starts the work. The work proceeds asynchronously.
    ANeuralNetworksEvent* run1_end = NULL;
    ANeuralNetworksExecution_startCompute(run1, &run1_end);
    // For our example, we have no other work to do and will just wait for the completion.
    ANeuralNetworksEvent_wait(run1_end);
    ANeuralNetworksEvent_free(run1_end);
    ANeuralNetworksExecution_free(run1);
}
