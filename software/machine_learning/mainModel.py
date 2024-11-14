import tensorflow as tf
import numpy as np
import pandas as pd #keep pandas here for now
#=======hyperparameters
num_features = 13 #discuss potential additions... refer to paper blake posted.. maybe add new metrics?
#perform a PCA to see which features I can exclude to make lighter  
num_classes = 17   
learning_rate = 0.001
epochs = 10
batch_size = 32

#this is dummy data until I get all data processed
X_train = 0#FIXME # I think 80/20 split is good... shuffle samples to get an even training
y_train = 0#FIXME   It should be a pretty long matrix that contains all ~13 features for each window
                 #from every subject. Expect an mxn matrix with m = (rawDataLength / 2.1) * numSubjects
                 # and n = ~13 

X_test = 0#    # 20 percent of x data
y_test = 0 #FIXME

#build model
model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=(num_features,)),
    tf.keras.layers.Dense(32, activation='relu'),  
    tf.keras.layers.Dense(16, activation='relu'),  
    tf.keras.layers.Dense(num_classes, activation='softmax') 
])

model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate), #compiling the model
              loss='categorical_crossentropy',
              metrics=['accuracy'])

#train and eval
model.fit(X_train, y_train, epochs=epochs, batch_size=batch_size, validation_data=(X_test, y_test))

loss, accuracy = model.evaluate(X_test, y_test)
print(f'Test accuracy: {accuracy * 100:.2f}%')

#SAVE MODEL to .tflite FILE FORMAT
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

with open('model.tflite', 'wb') as f:
    f.write(tflite_model)



#=================past this section is just for testing=================================

interpreter = tf.lite.Interpreter(model_content=tflite_model)
interpreter.allocate_tensors()

input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()



#================sample inference=============================
sample_input = X_test[0:1]
interpreter.set_tensor(input_details[0]['index'], sample_input)
interpreter.invoke()
output = interpreter.get_tensor(output_details[0]['index'])

print(f'Softmax output probabilities: {output}')
print(f'Predicted class: {np.argmax(output)}')
