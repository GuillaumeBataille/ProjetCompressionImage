from tkinter import *
from tkinterdnd2 import *
import tkinter as tk
from PIL import ImageTk, Image
import numpy as np
import cv2

class DragAndDrop:
    def __init__(self, master):
        self.master = master
        self.master.title('Drag and Drop Images')
        self.master.geometry('1200x1000')
        self.master.config(bg='white')

        self.image1_path = None
        self.image2_path = None
        self.disparity_generated = False # Nouvelle variable d'instance
        self.image4_path = None
        self.resized_disp = None
        self.resized_truedisp = None
        # create frames to organize widgets
        self.top_frame = Frame(self.master, bg='white')
        self.middle_frame = Frame(self.master, bg='white')
        self.bottom_frame = Frame(self.master, bg='white')
        self.right_frame = Frame(self.master, bg='white', width=200, height=1000)        
        # create canvas for left image
        self.canvas1 = Canvas(self.top_frame, bg='gray', width=350, height=350)
        self.canvas1.create_text(175, 175, text='Drop left image', font=(
            'Helvetica', 14), fill='white')
        self.canvas1.pack(side=LEFT, padx=10, pady=10)

        # create canvas for right image
        self.canvas2 = Canvas(self.top_frame, bg='gray', width=350, height=350)
        self.canvas2.create_text(175, 175, text='Drop right image', font=(
            'Helvetica', 14), fill='white')
        self.canvas2.pack(side=RIGHT, padx=10, pady=10)

        # create canvas for disparity map
        self.canvas3 = Canvas(self.bottom_frame, bg='gray', width=350, height=350)
        self.canvas3.pack(side=LEFT, padx=10, pady=10)

                # create button to use true disparity
        self.use_true_disparity_button1 = Button(self.middle_frame, state = DISABLED,text='Calculate PSNR',command = self.calculate_psnr)
        self.use_true_disparity_button1.pack(pady=8)
        
        self.use_true_disparity_button2 = Button(self.middle_frame, state = DISABLED,text='Find Best Parameter',command = self.calculate_best_psnr)
        self.use_true_disparity_button2.pack(pady=8)

        # create canvas for true disparity map
        self.canvas4= Canvas(self.bottom_frame, bg='gray', width=350, height=350)
        self.canvas4.create_text(175, 175, text='Drop true disparity map to find the best PSNR', font=(
            'Helvetica', 14), fill='white')

        # hide canvas4 initially
        self.canvas4.pack_forget()

        # method to show/hide canvas4 when button is clicked


        # register drop target for canvas1
        self.canvas1.drop_target_register(DND_FILES)
        self.canvas1.dnd_bind('<<Drop>>', self.drop_image1)

        # register drop target for canvas2
        self.canvas2.drop_target_register(DND_FILES)
        self.canvas2.dnd_bind('<<Drop>>', self.drop_image2)

        # register drop target for canvas2
        self.canvas4.drop_target_register(DND_FILES)
        self.canvas4.dnd_bind('<<Drop>>', self.drop_image4)

        # add button
        self.button = Button(self.middle_frame, text='Process images', state=DISABLED, command=self.process_images)
        self.button.pack(pady=8)

        #sliders 
        # create frame for scales
        self.scale_frame = Frame(self.middle_frame, bg='white')
        self.scale_frame.pack(side=LEFT, padx=20)

        # create block size scale
        self.block_size_scale = Scale(self.scale_frame, from_=3, to=100, orient=VERTICAL, label="Block Size",
                                    command=self.update_scale, troughcolor='gray')
        self.block_size_scale.set(15)
        self.block_size_scale.pack(side=LEFT, padx=2, pady=2)

        # create pre filter cap scale
        self.prefilter_cap_scale = Scale(self.scale_frame, from_=1, to=100, orient=VERTICAL, label="Pre Filter Cap",
                                        command=self.update_scale)
        self.prefilter_cap_scale.set(28)
        self.prefilter_cap_scale.pack(side=LEFT, padx=2, pady=2)


        #add label for psnr
        self.PSNRlabel = Label(self.master)
        self.PSNRlabel.pack()
        

        # pack frames
        self.top_frame.pack(side=TOP)
        self.middle_frame.pack(side=TOP)
        self.bottom_frame.pack(side=BOTTOM)
        self.right_frame.pack(side=RIGHT, padx=20, pady=20)

        # Initialiser les variables pour stocker les images
    def drop_image1(self, event):
        self.image1_path = event.data
        self.display_image(self.image1_path, self.canvas1)
        self.check_images_loaded()

    def drop_image2(self, event):
        self.image2_path = event.data
        self.display_image(self.image2_path, self.canvas2)
        self.check_images_loaded()
    
    def drop_image4(self, event):
        self.image4_path = event.data
        # Load the image and get its size
        image = Image.open(self.image4_path)
        image_width, image_height = image.size
        # Get the size of canvas3
        canvas_width, canvas_height = self.canvas3.winfo_width(), self.canvas3.winfo_height()
        # Compute the aspect ratio of the image and the canvas
        aspect_ratio = image_height / image_width
        canvas_aspect_ratio = canvas_height / canvas_width
        # Determine which dimension to resize the image by
        if aspect_ratio > canvas_aspect_ratio:
            # Height needs to be adjusted
            image_height = int(canvas_height)
            image_width = int(image_height / aspect_ratio)
        else:
            # Width needs to be adjusted
            image_width = int(canvas_width)
            image_height = int(image_width * aspect_ratio)
        
        # Resize the image
        image = cv2.resize(np.array(image), (image_width, image_height))
        self.resized_truedisp = image
        # Display the resized image on canvas4
        tk_disparity = ImageTk.PhotoImage(image=Image.fromarray((image).astype('uint8')))
        self.canvas4.delete('all')
        self.canvas4.create_image(0, 0, anchor=NW, image=tk_disparity)
        self.canvas4.image = tk_disparity
        self.check_images_loaded()

        self.use_true_disparity_button1.config(stat=NORMAL)
        self.use_true_disparity_button2.config(stat=NORMAL)

    def update_scale(self,value):
        # Change troughcolor to show that the scale is being moved
        self.block_size_scale.config(troughcolor='white')
        self.prefilter_cap_scale.config(troughcolor='white')

        # Update images based on scale values
        self.process_images()
    
    def show_true_disparity_canvas(self):
        self.canvas4.pack(side=RIGHT, padx=20, pady=20)

    def display_image(self, image_path, canvas):
        image = Image.open(image_path)
        w, h = image.size
        canvas.config(width=w, height=h)
        tk_image = ImageTk.PhotoImage(image.resize((w, h)))
        canvas.delete('all')
        canvas.create_image(0, 0, anchor=NW, image=tk_image)
        canvas.image = tk_image

    def check_images_loaded(self):
        if self.image1_path is not None and self.image2_path is not None:
            self.button.config(state=NORMAL)
        else:
            self.button.config(state=DISABLED)
     
    def calculate_psnr(self):
        # Convert the images to arrays
        image1 = self.resized_truedisp
        image2 = self.resized_disp
        
        # Resize image2 to match image1
        #imagesize = image1.shape[1] *  image1.shape[0] 
        image2 = cv2.resize(image2, (image1.shape[1], image1.shape[0]))
        
        # Calculate PSNR
        mse = np.mean(abs(image1 - image2) ** 2)
        if mse == 0:
            self.PSNRlabel.config(text = "PSNR: same image")
            return float('-inf')
        PIXEL_MAX = 255.0
        psnr =  10 * np.log10(mse)
        
        
        self.PSNRlabel.config(text = "PSNR: " + str(psnr))
        return psnr

    def process_images(self):
        if self.image1_path and self.image2_path:
            # Change troughcolor back to indicate that the scale is no longer being moved
            self.disparity_generated = True
            self.show_true_disparity_canvas()

            self.block_size_scale.config(troughcolor='gray')
            self.prefilter_cap_scale.config(troughcolor='gray')

            imgL = cv2.imread(self.image1_path)
            imgR = cv2.imread(self.image2_path)
            grayL = cv2.cvtColor(imgL, cv2.COLOR_BGR2GRAY)
            grayR = cv2.cvtColor(imgR, cv2.COLOR_BGR2GRAY)
            
            blocksize = self.block_size_scale.get()
            numDisp = self.prefilter_cap_scale.get()
            
            stereo = cv2.StereoSGBM_create(numDisparities=numDisp, blockSize=blocksize)
            disparity = stereo.compute(grayL, grayR)
            disparity_normalized = cv2.normalize(
                disparity, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
            # Crop the disparity map
            prefilter_cap_value = self.prefilter_cap_scale.get()
            image_height, image_width = disparity_normalized.shape
            disparity_normalized = disparity_normalized[:, prefilter_cap_value:image_width]
            #resize
            canvas_width, canvas_height = self.canvas3.winfo_width(), self.canvas3.winfo_height()
            aspect_ratio = image_height / image_width
            canvas_height = int(canvas_width * aspect_ratio)
            disparity_resized = cv2.resize(disparity_normalized, (canvas_width, canvas_height))
            self.resized_disp = disparity_resized
            tk_disparity = ImageTk.PhotoImage(image=Image.fromarray((disparity_resized*255).astype('uint8')))
            self.canvas3.delete('all')
            self.canvas3.create_image(0, 0, anchor=NW, image=tk_disparity)
            self.canvas3.image = tk_disparity
            

        else:
            print('Please drop both images first')
            
    def calculate_best_psnr(self):  
        
        temppsnr = 9999
        tempblocksize = 0 
        tempnumDisp = 0 
        
        for i in range(3, 100) :
            self.block_size_scale.set(i)
            self.process_images()
            psnr = self.calculate_psnr()
            
            if (psnr < temppsnr):
                temppsnr = psnr
                tempblocksize = i
                
          
        self.block_size_scale.set(tempblocksize) 
        print(temppsnr)
        for j in range(100 , 1 , -1) :
            self.prefilter_cap_scale.set(j)
            self.process_images()
            psnr = self.calculate_psnr()
        
            if (psnr < temppsnr):
                temppsnr = psnr
                tempnumDisp = j
         
        
        self.prefilter_cap_scale.set(tempnumDisp)
        self.PSNRlabel.config(text = "BEST PSNR: " + str(temppsnr))
      
        self.process_images()
            
if __name__ == '__main__':
    root = TkinterDnD.Tk()
    DragAndDrop(root)
    root.mainloop()
    
