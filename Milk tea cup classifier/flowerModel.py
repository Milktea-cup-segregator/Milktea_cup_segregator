import tensorflow as tf
from tensorflow import keras


# 定义模型类
class mnistModel():
    # 初始化结构
    def __init__(self, checkpoint_path, log_path, model_path, num_classes, img_width, img_height):
        # checkpoint 权重保存地址
        self.checkpoint_path = checkpoint_path
        # 训练日志保存地址
        self.log_path = log_path
        # 训练模型保存地址
        self.model_path = model_path
        # 数据统一大小并归一处理
        resize_and_rescale = tf.keras.Sequential([
            keras.layers.Resizing(img_width, img_height),
            keras.layers.Rescaling(1. / 255)
        ])
        # 数据增强
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

        # 初始化模型结构
        self.model = keras.Sequential([
            resize_and_rescale,
            data_augmentation,
            keras.layers.Conv2D(32, (3, 3),
                                kernel_initializer=keras.initializers.truncated_normal(stddev=0.05),
                                kernel_regularizer=keras.regularizers.l2(0.001),
                                padding='same',
                                activation='relu'),
            keras.layers.MaxPooling2D(2, 2),
            keras.layers.Conv2D(32, (3, 3),
                                kernel_initializer=keras.initializers.truncated_normal(stddev=0.05),
                                kernel_regularizer=keras.regularizers.l2(0.001),
                                padding='same',
                                activation='relu'),
            keras.layers.MaxPooling2D(2, 2),
            keras.layers.Conv2D(32, (3, 3),
                                kernel_initializer=keras.initializers.truncated_normal(stddev=0.05),
                                kernel_regularizer=keras.regularizers.l2(0.001),
                                padding='same',
                                activation='relu'),
            keras.layers.MaxPooling2D(2, 2),
            keras.layers.Flatten(),
            keras.layers.Dense(1024,
                               kernel_initializer=keras.initializers.truncated_normal(stddev=0.05),
                               kernel_regularizer=keras.regularizers.l2(0.001),
                               activation=tf.nn.relu),
            keras.layers.Dropout(0.2),
            keras.layers.Dense(256,
                               kernel_initializer=keras.initializers.truncated_normal(stddev=0.05),
                               kernel_regularizer=keras.regularizers.l2(0.001),
                               activation=tf.nn.relu),
            keras.layers.Dense(num_classes, activation='softmax')
        ])

    # 编译模型
    def compile(self):
        # 输出模型摘要
        self.model.build(input_shape=(None, 180, 180, 3))
        self.model.summary()
        # 定义训练模式
        self.model.compile(optimizer='adam',
                           loss='sparse_categorical_crossentropy',
                           metrics=['accuracy'])

    # 训练模型
    def train(self, train_ds, val_ds):
        # tensorboard 训练日志收集
        tensorboard = keras.callbacks.TensorBoard(log_dir=self.log_path)

        # 训练过程保存 Checkpoint 权重，防止意外停止后可以继续训练
        model_checkpoint = keras.callbacks.ModelCheckpoint(self.checkpoint_path, # 保存模型的路径
                                                            # moniter='val_loss', # 被监测的数据。
                                                            verbose=0, # 详细信息模式，0 或者 1
                                                           save_best_only=True, # 如果 True, 被监测数据的最佳模型就不会被覆盖
                                                           save_weights_only=True,
                                                           mode='auto',
                                                           period=3
                                                           )

        # 填充数据，迭代训练
        self.model.fit(
            train_ds, # 训练集
            validation_data=val_ds, # 验证集
            epochs=200, # 迭代周期
            verbose = 2, # 训练过程的日志信息显示，一个epoch输出一行记录
            callbacks=[tensorboard, model_checkpoint]
        )
        # 保存训练模型
        self.model.save(self.model_path)

    def evaluate(self, val_ds):
        # 评估模型
        test_loss, test_acc =self.model.evaluate(val_ds)
        return test_loss, test_acc
