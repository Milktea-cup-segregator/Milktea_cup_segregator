import tensorflow as tf
import pathlib
from tensorflow import keras
from milkteaModel import mnistModel


# 数据预处理
def getData():
    # 加载数据集
    path = "D:/milkTea/milkTeaCup"
    # 解析目录
    data_dir = pathlib.Path(path)

    # keras 加载数据集
    batch_size = 32
    img_height = 180
    img_width = 180

    # 使用80% 的图像进行训练，20% 的图像进行 验证。
    class_names = ['yes', 'no',]
    train_ds = keras.preprocessing.image_dataset_from_directory(
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
        color_mode='rgb',
    )

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
        color_mode='rgb',
    )

    AUTOTUNE = tf.data.AUTOTUNE
    train_ds = train_ds.cache().prefetch(buffer_size=AUTOTUNE)
    val_ds = val_ds.cache().prefetch(buffer_size=AUTOTUNE)
    return train_ds, val_ds, len(class_names), img_width, img_height


def main():
    # 加载数据集
    train_ds, val_ds, num_classes, img_width, img_height = getData()

    checkpoint_path = './checkout/'
    log_path = './log'
    model_path = './model/model.h5'

    # 构建模型
    model = mnistModel(checkpoint_path, log_path, model_path, num_classes, img_width, img_height)
    # 编译模型
    model.compile()
    # 训练模型
    model.train(train_ds, val_ds)
    # 评估模型
    test_loss, test_acc = model.evaluate(val_ds)
    print(test_loss, test_acc)


if __name__ == '__main__':
    main()
