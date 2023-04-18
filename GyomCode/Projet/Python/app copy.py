from tkinter import *
from tkinterdnd2 import *
from PIL import ImageTk, Image
import cv2


class DragAndDrop:
    def __init__(self, master):
        self.master = master
        self.master.title('Disparity Map Generator')
        self.master.geometry('1920x1080')
        self.master.config(bg='gold')

        self.image1_path = None
        self.image2_path = None

        self.image1_label = Label(self.master, text='Drop Image 1 Here', bg='#fcba03', font=(
            'Helvetica', 14), height=5, width=50)
        self.image1_label.pack(pady=20)

        self.image2_label = Label(self.master, text='Drop Image 2 Here', bg='#fcba03', font=(
            'Helvetica', 14), height=5, width=50)
        self.image2_label.pack(pady=20)

        self.disparity_button = Button(self.master, text='Generate Disparity Map', font=(
            'Helvetica', 14), command=self.generate_disparity_map)
        self.disparity_button.pack(pady=20)

        self.disparity_label = Label(self.master, bg='gray')
        self.disparity_label.pack(pady=20)

        # register drop target for image1_label
        self.image1_label.drop_target_register(DND_FILES)
        self.image1_label.dnd_bind('<<Drop>>', self.drop_image1)

        # register drop target for image2_label
        self.image2_label.drop_target_register(DND_FILES)
        self.image2_label.dnd_bind('<<Drop>>', self.drop_image2)

    def drop_image1(self, event):
        self.image1_path = event.data
        self.display_image(self.image1_path, self.image1_label)

    def drop_image2(self, event):
        self.image2_path = event.data
        self.display_image(self.image2_path, self.image2_label)

    def display_image(self, image_path, label):
        image = Image.open(image_path)
        tk_image = ImageTk.PhotoImage(image)
        label.configure(image=tk_image)
        label.image = tk_image

    def generate_disparity_map(self):
        if self.image1_path is None or self.image2_path is None:
            return

        img1 = cv2.imread(self.image1_path, cv2.IMREAD_GRAYSCALE)
        img2 = cv2.imread(self.image2_path, cv2.IMREAD_GRAYSCALE)

        # create stereo object
        stereo = cv2.StereoBM_create(numDisparities=64, blockSize=15)

        # compute disparity map
        disparity_map = stereo.compute(img1, img2)

        # normalize and convert disparity map to 8-bit image
        min_val, max_val, _, _ = cv2.minMaxLoc(disparity_map)
        disparity_map_norm = cv2.normalize(
            disparity_map, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)

        # display disparity map
        tk_disparity_map = ImageTk.PhotoImage(
            image=Image.fromarray(disparity_map_norm))
        self.disparity_label.configure(image=tk_disparity_map)
        self.disparity_label.image = tk_disparity_map


if __name__ == '__main__':
    root = TkinterDnD.Tk()
    DragAndDrop(root)
    root.mainloop()
