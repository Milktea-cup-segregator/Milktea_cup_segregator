import pathlib
import matplotlib.pyplot as plt
from tensorflow import keras
import tensorflow as tf

# path = "D:/milkTea/flower_photos"
# # 解析目录
# data_dir = pathlib.Path(path)
# print(len(list(data_dir.glob('*/*.jpg'))))
#
# sunflowers = list(data_dir.glob('sunflowers/*'))
# print(str(sunflowers[0]))
# plt.imshow(plt.imread(str(sunflowers[0])), cmap=plt.cm.get_cmap('gray'))
# plt.show()

plt.rcParams['font.sans-serif'] = ['SimHei']

path = "D:/milkTea/flower_photos"
# 解析目录
data_dir = pathlib.Path(path)

batch_size = 32
img_height = 180
img_width = 180

# 使用80%的图像进行训练，20%的图像进行验证。
class_names = ['daisy', 'dandelion', 'roses', 'sunflowers', 'tulips']
class_names_cn = ['雏菊', '蒲公英', '玫瑰', '向日葵', '郁金香']

train_ds = keras.utils.image_dataset_from_directory(
    directory=data_dir,
    validation_split=0.2,
    subset="training",
    image_size=(img_height, img_width),
    batch_size=batch_size,
    shuffle=True,
    seed=123,
    interpolation='bilinear',
    crop_to_aspect_ratio=True,
    labels='inferred',
    class_names=class_names,
    color_mode='rgb',
)

# 从缓冲区查看图片
AUTOTUNE = tf.data.AUTOTUNE
train_ds = train_ds.cache().prefetch(buffer_size=AUTOTUNE)

# 数据增强
# model = keras.Sequential([
#     # 归一化
#     keras.layers.Rescaling(1. / 255),
#     keras.layers.RandomRotation(0.2)
# ])

# 缩放和归一化
IMG_SIZE = 180
resize_and_rescale = tf.keras.Sequential([
    tf.keras.layers.Resizing(IMG_SIZE, IMG_SIZE),
    tf.keras.layers.Rescaling(1. / 255)
])

# 图像增强
data_augmentation = tf.keras.Sequential([
    # 翻转
    tf.keras.layers.RandomFlip("horizontal_and_vertical"),
    # 旋转
    tf.keras.layers.RandomRotation(0.2),
    # 对比度
    tf.keras.layers.RandomContrast(0.3),
    # 随记播放
    tf.keras.layers.RandomZoom(height_factor=0.3, width_factor=0.3),
])

model = keras.Sequential([
    resize_and_rescale,
    data_augmentation
])

# 遍历图像
# 总批次大小
print('训练总批次', len(train_ds))
# 获取第一个批次数据
plt.figure(figsize=(10, 10))
for image_batch, labels_batch in train_ds.take(1):
    # for i in range(9):
    #     print('图片批次shape：', image_batch.shape)
    #     print('标签批次shape：', labels_batch.shape)
    #     print('单图片shape：', image_batch[i].shape)
    #     ax = plt.subplot(3, 3, i+1)
    #     plt.imshow(image_batch[i].numpy().astype("uint8"))
    #     plt.axis("off")
    # plt.show()
    for i in range(len(image_batch)):
        plt.figure(figsize=(10, 10))
        for j in range(9):
            plt.subplot(3, 3, j+1)
            # augmented_image = model.predict(tf.expand_dims(image_batch[i], 0))
            augmented_image = model(tf.expand_dims(image_batch[i], 0))
            print(augmented_image[0].shape)
            plt.imshow(augmented_image[0])
            plt.title(class_names_cn[labels_batch[i]])
            plt.axis('off')
        plt.show()


