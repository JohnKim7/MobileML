#include <jni.h>
#include <string>
#include <android/NeuralNetworks.h>
#include <float.h>
#include <android/log.h>
#include <sstream>
#include <iomanip>
#include <fcntl.h>


extern "C" JNIEXPORT jfloat
JNICALL
Java_gisia_martin_com_perceptron_MainActivity_modelTraining(JNIEnv *env, jobject instance) {

    //Datos de entrenamiento. (x,x,y) representa los 2 valores de entrada de la operacion OR e Y representa la salida;
    float dataTraining[4][3] = {{0.0,0.0,0.0},{0.0,1.0,0.0},{1.0,0.0,0.0},{1.0,1.0,1.0}};

    ANeuralNetworksModel* model = NULL;
    ANeuralNetworksModel_create(&model);


    // In our example, all our tensors are matrices of dimension [3][4].
    ANeuralNetworksOperandType entradas, pesos, sal, activacion, bias;
    entradas.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    entradas.scale = 0.f;
    entradas.zeroPoint = 0;
    entradas.dimensionCount = 2;
    uint32_t dimsInput[2] = {1,2};
    entradas.dimensions = dimsInput;

    pesos.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    pesos.scale = 0.f;
    pesos.zeroPoint = 0;
    pesos.dimensionCount= 2;
    uint32_t dimsPesos[2] = {1,2};
    pesos.dimensions = dimsPesos;

    bias.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    bias.scale = 0.f;
    bias.zeroPoint = 0;
    bias.dimensionCount= 1;
    uint32_t dimsBias[1] = {1};
    bias.dimensions = dimsBias;

    /*sal.type= ANEURALNETWORKS_FLOAT32;
    sal.scale = 0.f;
    sal.zeroPoint = 0;
    sal.dimensionCount = 0;
    sal.dimensions = nullptr;*/
    sal.type= ANEURALNETWORKS_TENSOR_FLOAT32;
    sal.scale = 0.f;
    sal.zeroPoint = 0;
    sal.dimensionCount = 2;
    uint32_t dimsSal[2] = {1,1};
    sal.dimensions = dimsSal;

    activacion.type = ANEURALNETWORKS_INT32;
    activacion.scale = 0.f;
    activacion.zeroPoint = 0;
    activacion.dimensionCount = 0;
    activacion.dimensions = nullptr;






    // Now we add the seven operands, in the same order defined in the diagram.
    ANeuralNetworksModel_addOperand(model, &entradas);  // operand 0
    ANeuralNetworksModel_addOperand(model, &pesos);  // operand 1
    ANeuralNetworksModel_addOperand(model, &bias);  // operand 2
    ANeuralNetworksModel_addOperand(model, &activacion);  // operand 3
    ANeuralNetworksModel_addOperand(model, &sal); // operand 4

    /*uint32_t ent[3] = {1,1,1};
    const void *bufferEnt = ent;
    ANeuralNetworksModel_setOperandValue(model, 0, bufferEnt, 3);*/


    float pes[2] = {1.0,1.0};
    //ANeuralNetworksModel_setOperandValue(model, 1, &pes, sizeof(pes));

    float biasBuf[1] = {-0.5};
    //ANeuralNetworksModel_setOperandValue(model, 2, &biasBuf, sizeof(biasBuf));


    // We set the values of the activation operands, in our example operands 2 and 5.
    int32_t noneValue = ANEURALNETWORKS_FUSED_NONE;
    ANeuralNetworksModel_setOperandValue(model, 3, &noneValue, sizeof(noneValue));


    // We have two operations in our example.
    // The first consumes operands 1, 0, 2, and produces operand 4.
    uint32_t addInputIndexes[4] = {0, 1,2,3};
    uint32_t addOutputIndexes[1] = {4};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_FULLY_CONNECTED, 4, addInputIndexes, 1, addOutputIndexes);

    // Our model has one input (0) and one output (6).
    uint32_t modelInputIndexes[3] = {0,1,2};
    uint32_t modelOutputIndexes[1] = {4};
    ANeuralNetworksModel_identifyInputsAndOutputs(model, 3, modelInputIndexes, 1 ,modelOutputIndexes);

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
    // Set the single input to our sample model. Since it is small, we won’t use a memory buffer.


    float  n=0.0;
    float  e=0.0;
    float  d=0.0;
    //Tasa de aprendizaje
    float r=0;
    r=0.1;
    while (true){
        int contErrores=0;
        for (int i=0; i <4; i++){
            float myInput[2]= {dataTraining[i][0], dataTraining[i][1]};
            float salidaDeseada = {dataTraining[i][2]};

            ANeuralNetworksExecution_setInput(run1, 0, NULL, myInput, sizeof(myInput));
            ANeuralNetworksExecution_setInput(run1, 1, NULL, pes, 2);
            ANeuralNetworksExecution_setInput(run1, 2, NULL, biasBuf, 1);
            // Set the output.
            float myOutput[1];
            ANeuralNetworksExecution_setOutput(run1, 0, NULL, myOutput, sizeof(myOutput));
            // Starts the work. The work proceeds asynchronously.
            ANeuralNetworksEvent* run1_end = NULL;
            ANeuralNetworksExecution_startCompute(run1, &run1_end);
            // For our example, we have no other work to do and will just wait for the completion.
            ANeuralNetworksEvent_wait(run1_end);
            //ANeuralNetworksEvent_free(run1_end);
            //ANeuralNetworksExecution_free(run1);


            if (myOutput[0] >= 0.0){
                n=1.0;
            } else{
                n=0.0;
            }
            e=salidaDeseada-n;
            if (e!=0.0){
                contErrores++;
                d= r*e;
                pes[0] = pes[0] + myInput[0]*d;
                pes[1] = pes[1] + myInput[1]*d;
                biasBuf[0] = biasBuf[0] + d;
            }
        }
        if (contErrores==0){
            break;
        }
    }
    
    
    float myInput[2] = {1,1};
    ANeuralNetworksExecution_setInput(run1, 0, NULL, myInput, sizeof(myInput));
    // Set the output.
    float myOutput[1];
    ANeuralNetworksExecution_setOutput(run1, 0, NULL, myOutput, sizeof(myOutput));
    // Starts the work. The work proceeds asynchronously.
    ANeuralNetworksEvent* run1_end = NULL;
    ANeuralNetworksExecution_startCompute(run1, &run1_end);
    // For our example, we have no other work to do and will just wait for the completion.
    ANeuralNetworksEvent_wait(run1_end);
    ANeuralNetworksEvent_free(run1_end);
    ANeuralNetworksExecution_free(run1);


    /*float result = 0;
    for (int i = 0; i < sizeof(myOutput); ++i) {
        result = result + myOutput[i];
    }
    return result;*/
    return myOutput[0];
}