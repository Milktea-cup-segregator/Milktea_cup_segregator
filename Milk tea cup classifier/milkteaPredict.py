import tensorflow as tf
import pathlib
from tensorflow import keras
import matplotlib.pyplot as plt
import cv2

plt.rcParams['font.sans-serif'] = ['SimHei']

def main():
    # 加载数据集
    path = "D:/milkTea/milkTeaCup"
    # 解析目录
    data_dir = pathlib.Path(path)

    # keras 加载数据集
    batch_size = 32
    img_height = 180
    img_width = 180

    # 使用 80% 的图像进行训练， 20%的图像进行验证。
    class_names = ['yes', 'no']
    class_names_cn = ['是奶茶杯', '非奶茶杯']

    val_ds = keras.preprocessing.image_dataset_from_directory(
        directory=data_dir,
        validation_split=0.2,
        subset="training",
        image_size=(img_height, img_width),
        batch_size=batch_size,
        shuffle=True,
        seed=123,
        interpolation='bilinear',
        # crop_to_aspect_ratio=True,
        labels='inferred',
        class_names=class_names,
        # color_mode='rgb',
    )

    model = keras.models.load_model('./model/model.h5')
    # 获取第一个批次数据
    for image_batch, labels_batch in val_ds.take(1):
        plt.figure(figsize=(20, 20))
        for i in range(3):
            plt.subplot(3, 3, i+1)
            softmax = model.predict(tf.expand_dims(image_batch[i], 0))
            y_label = tf.argmax(softmax, axis=1).numpy()[0]
            plt.imshow(image_batch[i].numpy().astype("uint8"))
            plt.title('预测结果：' + class_names_cn[y_label] + '，概率：' + str("%.2f" % softmax[0][y_label]) + ', 真实结果：' +
                      class_names_cn[labels_batch[i]])
            plt.axis('off')
        plt.show()

if __name__ == "__main__":
    main()