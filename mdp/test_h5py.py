#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import numpy as np
import h5py
from PIL import Image, ImageTk,ImageOps,ImageEnhance
import tkinter
import tkinter.messagebox as messagebox
from tkinter import filedialog
import os

'''
def choose_file():
    file_list = os.listdir('.')
    root2 = tkinter.Tk()
    frame1 = tkinter.Frame(root2)
    frame1.pack()
    frame2 = tkinter.Frame(root2)
    frame2.pack()
    tkinter.Label(frame1,text = 'Input your directory').pack()

    directory_value = tkinter.StringVar()
    directory_entry = tkinter.Entry(frame1, width=30, textvariable=directory_value)
    directory_entry.pack()
    directory_value.set(os.path.abspath('.'))

    def change_file_menu(*args):
        nonlocal file_list
        file_name.set('')
        file_menu['menu'].delete(0,'end')
        file_list.clear()
        try:
            file_list = os.listdir(directory_value.get())
        except FileNotFoundError:
            return

        for x in file_list:
            file_menu['menu'].add_command(label=x, command=tkinter._setit(file_name, x))

    directory_value.trace('w', change_file_menu)


    tkinter.Label(frame2, text='choose the hdf5 file').pack(side = 'left')
    file_name = tkinter.StringVar()
    file_menu = tkinter.OptionMenu(frame2, file_name, *file_list)
    file_menu.pack(side = 'left')
    file_menu.config(width=10)
    def give_name():
        viewer = Image_display(file_name.get())
        viewer.GUI()
    button = tkinter.Button(root2, text='open the file', command = give_name)
    button.pack()
    root2.mainloop()

'''

class Image_display(object):

    #def __init__(self,file_name):
    #    self.file_name = file_name


    def preperation_for_display(self,mode = 'RGB',channel = 0):
        self.previous_img = None
        if mode == 'RGB':
            if len(self.array.shape) < 3:
                print('Not a RGB image\n')
                return
            self.im = Image.fromarray(self.array, mode='RGB')
        elif mode == 'gray':
            if len(self.array.shape) == 3:
                array_gray = self.array[:, :, channel]
            else:
                array_gray = self.array
            self.im = Image.fromarray(array_gray, mode='L')


    def genetate_pixel_value(self,x_coord,y_coord):
        current_array = np.asarray(self.im)
        if len(self.array.shape) == 3 and len(current_array.shape) == 3:
            messagebox.showinfo(title='pixel value', message = 'dataset value\nR value = %d\nG value = %d\nB value = %d\n\ndisplayed image\nR value = %d\nG value = %d\nB value = %d' %
            (self.array[y_coord, x_coord, 0], self.array[y_coord, x_coord, 1], self.array[y_coord, x_coord, 2],current_array[y_coord,x_coord,0],current_array[y_coord,x_coord,1],current_array[y_coord,x_coord,2]))
        elif len(self.array.shape) == 3 and len(current_array.shape) == 2:
            messagebox.showinfo(title='pixel value', message='dataset value\nR value = %d\nG value = %d\nB value = %d\n\ndisplayed image\npixel value = %d' %
            (self.array[y_coord, x_coord, 0], self.array[y_coord, x_coord, 1], self.array[y_coord, x_coord, 2],current_array[y_coord,x_coord]))
        else:
            messagebox.showinfo(title='pixel value', message='dataset value\npixel value = %d\n\ndisplayed image\npixel value = %d' % (self.array[y_coord, x_coord],current_array[y_coord,x_coord]))


    def show_image(self):
        root1 = tkinter.Tk()
        root1.title('test_file')

        self.canvas = tkinter.Canvas(root1,width = self.array.shape[1],height = self.array.shape[0])
        photo = ImageTk.PhotoImage(master=self.canvas, image = self.im)
        self.canvas_image_id = self.canvas.create_image(0,0,anchor='nw',image = photo)
        self.canvas.image = photo

        def callback1(e):
            self.canvas.itemconfig(canvas_id,text = ('x coordinate:%d \ny coordinate:%d') % (e.x,e.y))


        def callback2(e):
            self.genetate_pixel_value(e.x,e.y)

        self.canvas.bind('<Motion>',callback1)
        self.canvas.bind('<Button-1>',callback2)
        self.canvas.pack()

        canvas_id = self.canvas.create_text(10, 10, anchor="nw")
        self.canvas.itemconfig(canvas_id, text="x coordinate:\ny coordinate")

        if (self.previous_img != None):
            def get_back_image():
                self.im = self.previous_img
                self.previous_img = None
                root1.destroy()
                self.show_image()
            button = tkinter.Button(root1,text = 'Get_back',bg = 'blue',command = get_back_image)
            self.canvas.create_window((self.array.shape[1]-30)//2, self.array.shape[0]-30, anchor='nw', window=button)


        root1.mainloop()


    def GUI(self):
        #f = h5py.File(self.file_name, 'r')
        #        self.array = np.array(f[image_name][:])

        image_list = []
        raster_list = []
        meta_group_list = []
        f = 0


        root = tkinter.Tk()
        frame_first = tkinter.Frame(root)
        frame_first.pack()
        tkinter.Label(frame_first,text = 'file_directory').pack(side = 'left')

        directory_and_file_value = tkinter.StringVar()
        directory_and_file_entry = tkinter.Entry(frame_first, width=30, textvariable=directory_and_file_value)
        directory_and_file_entry.pack(side = 'left')

        def openfile():
            nonlocal directory_and_file_value
            nonlocal f
            nonlocal image_list
            nonlocal meta_group_list
            nonlocal meta_group_menu
            nonlocal meta_group_name
            nonlocal frame
            nonlocal image_menu
            nonlocal frame2
            nonlocal image_name
            nonlocal raster_name
            nonlocal R_name
            nonlocal G_name
            nonlocal B_name
            nonlocal meta_dataset_name
            nonlocal group_metadata_button
            nonlocal image_menu1
            nonlocal image_menu2
            nonlocal image_menu3
            nonlocal image_menu4
            nonlocal meta_dataset_menu

            r = filedialog.askopenfilename(title='open the file')
            if (r==''):
                return
            directory_and_file_value.set(r)
            current_directory,self.file_name = os.path.split(r)
            os.chdir(current_directory)

            image_name.set('')
            raster_name.set('')
            R_name.set('')
            G_name.set('')
            B_name.set('')
            meta_dataset_name.set('')
            meta_group_name.set('')
            image_menu1['menu'].delete(0, 'end')
            image_menu2['menu'].delete(0, 'end')
            image_menu3['menu'].delete(0, 'end')
            image_menu4['menu'].delete(0, 'end')
            meta_dataset_menu['menu'].delete(0, 'end')
            meta_group_menu['menu'].delete(0,'end')
            image_menu['menu'].delete(0,'end')

            f = h5py.File(self.file_name, 'r')

            image_list.clear()
            meta_group_list.clear()
            meta_group_list.append('file')
            for x in f.keys():
                image_list.append(x)
                meta_group_list.append(x)

            meta_group_menu.destroy()
            meta_group_menu = tkinter.OptionMenu(frame, meta_group_name, *meta_group_list, command=update_meta_dataset_name)
            meta_group_menu.pack(side='left')
            meta_group_menu.config(width=7)
            meta_group_name.set('file')
            group_metadata_button.pack(side='right')

            image_menu.destroy()
            image_menu = tkinter.OptionMenu(frame2, image_name, *image_list, command=update_raster_name)
            image_menu.config(width=8)
            image_menu.pack()


        browse_button = tkinter.Button(frame_first, text='Browse', command=openfile)
        browse_button.pack(side = 'left')

        frame = tkinter.Frame(root)
        frame.pack()
        frame11 = tkinter.Frame(root)
        frame11.pack()
        tkinter.Label(frame,text = 'metadata group').pack(side = 'left')
        meta_group_name = tkinter.StringVar()
        def update_meta_dataset_name(value):
            meta_dataset_name.set('')
            meta_dataset_menu['menu'].delete(0,'end')
            for x in f[value].keys():
                meta_dataset_menu['menu'].add_command(label=x, command=tkinter._setit(meta_dataset_name, x))

        meta_group_menu = tkinter.OptionMenu(frame,meta_group_name,value='',command=update_meta_dataset_name)
        meta_group_menu.pack(side = 'left')
        meta_group_menu.config(width = 7)
        meta_group_name.set('file')

        def show_group_meta():
            string = ''
            if meta_group_name.get() == 'file':
                for x in f.attrs.items():
                    string = string + str(x) + '\n'

            else:
                for x in f[meta_group_name.get()].attrs.items():
                    string = string + str(x) + '\n'

            messagebox.showinfo(title='metadata', message='%s' % string)


        group_metadata_button = tkinter.Button(frame,text = 'group metadata',command = show_group_meta)
        group_metadata_button.pack(side='left')

        tkinter.Label(frame11,text = 'metadata dataset').pack(side = 'left')
        meta_dataset_name = tkinter.StringVar()
        meta_dataset_menu = tkinter.OptionMenu(frame11,meta_dataset_name,value = '')
        meta_dataset_menu.pack(side = 'left')
        meta_dataset_menu.config(width = 7)
        def show_dataset_meta():
            string = ''
            for x in f[meta_group_name.get()][meta_dataset_name.get()].attrs.items():
                string = string + str(x) + '\n'

            messagebox.showinfo(title = 'metadata',message = '%s' % string)

        tkinter.Button(frame11, text='dataset metadata', command=show_dataset_meta).pack(side='left')

        frame1 = tkinter.Frame(root, height=100, width=100)
        frame1.pack(side='top')
        frame2 = tkinter.Frame(root, height=100, width=100)
        frame2.pack(side='top')
        frame3 = tkinter.Frame(root, height=100, width=100)
        frame3.pack(side='top')
        frame4 = tkinter.Frame(root, height=100, width=100)
        frame4.pack(side='top')

        image_name = tkinter.StringVar(root)

        def update_raster_name(value):
            raster_name.set('')
            R_name.set('')
            G_name.set('')
            B_name.set('')
            image_menu1['menu'].delete(0, 'end')
            image_menu2['menu'].delete(0, 'end')
            image_menu3['menu'].delete(0, 'end')
            image_menu4['menu'].delete(0, 'end')
            raster_list.clear()
            for x in f[value].keys():
                raster_list.append(x)
            change_raster_menu()

        tkinter.Label(frame2, text='image name').pack(side='left')

        image_menu = tkinter.OptionMenu(frame2, image_name, value='', command=update_raster_name)
        image_menu.config(width=8)
        image_menu.pack()

        tkinter.Label(frame3, text='raster name').pack(side='left')
        raster_name = tkinter.StringVar()
        image_menu1 = tkinter.OptionMenu(frame3, raster_name, value='')
        image_menu1.config(width=8)
        image_menu1.pack()

        R_name = tkinter.StringVar()
        G_name = tkinter.StringVar()
        B_name = tkinter.StringVar()
        tkinter.Label(frame4, text='R channel').pack(side='left')
        image_menu2 = tkinter.OptionMenu(frame4, R_name, value='')
        image_menu2.pack(side='left')
        image_menu2.config(width=8)
        tkinter.Label(frame4, text='G channel').pack(side='left')
        image_menu3 = tkinter.OptionMenu(frame4, G_name, value='')
        image_menu3.pack(side='left')
        image_menu3.config(width=8)
        tkinter.Label(frame4, text='B channle').pack(side='left')
        image_menu4 = tkinter.OptionMenu(frame4, B_name, value='')
        image_menu4.pack(side='left')
        image_menu4.config(width=8)

        def change_raster_menu():

            for choice in raster_list:
                image_menu1['menu'].add_command(label=choice, command=tkinter._setit(raster_name, choice))
                image_menu2['menu'].add_command(label=choice, command=tkinter._setit(R_name, choice))
                image_menu3['menu'].add_command(label=choice, command=tkinter._setit(G_name, choice))
                image_menu4['menu'].add_command(label=choice, command=tkinter._setit(B_name, choice))

        def change_enhancement_menu(*args):
            raster_name.set('')
            R_name.set('')
            G_name.set('')
            B_name.set('')

            nonlocal frame5
            frame5.destroy()

            frame5 = tkinter.Frame(root, height=100, width=100)
            frame5.pack(side='top')
            nonlocal reference_label
            nonlocal reference_entry
            nonlocal channel_label
            nonlocal channel_menu
            nonlocal enhancement_menu
            nonlocal enhancement_button
            nonlocal enhancement_value
            nonlocal enhancement_name
            nonlocal reference_value1
            nonlocal reference_value2

            tkinter.Label(frame5, text='enhancement method').pack(side='left')
            enhancement_list = ['linear stretch', 'histogram equalization', 'brightness change', 'contrast change']
            enhancement_value = tkinter.StringVar()
            enhancement_value.set('linear stretch')
            enhancement_menu = tkinter.OptionMenu(frame5, enhancement_value, *enhancement_list, command=change_function)
            enhancement_menu.pack(side='left')

            reference_label = tkinter.Label(frame5, text='k and b')
            reference_label.pack(side='left')
            reference_value1 = tkinter.DoubleVar()
            reference_entry = tkinter.Frame(frame5)
            reference_entry.pack(side='left')
            reference_entry1 = tkinter.Entry(reference_entry, width=5, textvariable=reference_value1)
            reference_entry1.pack(side='left')
            reference_value1.set(1.0)

            reference_value2 = tkinter.DoubleVar()
            reference_entry2 = tkinter.Entry(reference_entry, width=5, textvariable=reference_value2)
            reference_entry2.pack(side='left')
            reference_value2.set(0)

            enhancement_name = tkinter.StringVar()
            channel_label = tkinter.Label(frame5, text='channel')
            channel_label.pack(side='left')
            if v.get()==0:
                enhancement_name.set('gray')
                channel_menu = tkinter.OptionMenu(frame5, enhancement_name, value='gray')
            else:
                enhancement_name.set('R')
                channel_menu = tkinter.OptionMenu(frame5, enhancement_name, 'R','G','B')
            channel_menu.pack(side='left')
            channel_menu.config(width=6)
            enhancement_button = tkinter.Button(frame5, text='apply',command = apply_enhancement)
            enhancement_button.pack(side = 'left')

        v = tkinter.IntVar()
        v.set(0)
        v.trace('w', change_enhancement_menu)
        rad1 = tkinter.Radiobutton(frame1, variable=v, text='gray', value=0)
        rad1.pack(side='left')
        rad2 = tkinter.Radiobutton(frame1, variable=v, text='RGB', value=1)
        rad2.pack(side='left')

        def make_sure():
            if v.get() == 0:
                self.array = np.array(f[image_name.get()][raster_name.get()][:])
                self.preperation_for_display(mode='gray')
            elif v.get() == 1:
                a_array = np.array(f[image_name.get()][R_name.get()][:])
                b_array = np.array(f[image_name.get()][G_name.get()][:])
                c_array = np.array(f[image_name.get()][B_name.get()][:])
                self.array = np.empty((a_array.shape[0],a_array.shape[1],3),dtype = a_array.dtype)
                self.array[:,:,0] = a_array
                self.array[:,:,1] = b_array
                self.array[:,:,2] = c_array
                self.preperation_for_display(mode='RGB')
            self.show_image()

        certain_button = tkinter.Button(root, text='display image', command=make_sure)
        certain_button.pack()

        frame5 = tkinter.Frame(root, height=100, width=100)
        frame5.pack(side='top')

        tkinter.Label(frame5, text='enhancement method').pack(side='left')
        enhancement_list = ['linear stretch', 'histogram equalization', 'brightness change', 'contrast change']
        enhancement_value = tkinter.StringVar()
        enhancement_value.set('linear stretch')

        def change_function(value):
            nonlocal reference_label
            nonlocal reference_entry
            nonlocal channel_label
            nonlocal channel_menu
            nonlocal enhancement_button
            nonlocal enhancement_name
            nonlocal reference_value1
            nonlocal reference_value2
            nonlocal reference_value3
            nonlocal reference_value4
            nonlocal reference_value5

            reference_label.destroy()
            reference_entry.destroy()
            channel_label.destroy()
            channel_menu.destroy()
            enhancement_button.destroy()

            if value == 'linear stretch':
                reference_label = tkinter.Label(frame5, text='k and b')
                reference_label.pack(side='left')
                reference_value1 = tkinter.DoubleVar()
                reference_entry = tkinter.Frame(frame5)
                reference_entry.pack(side='left')
                reference_entry1 = tkinter.Entry(reference_entry, width=5, textvariable=reference_value1)
                reference_entry1.pack(side='left')
                reference_value1.set(1.0)

                reference_value2 = tkinter.DoubleVar()
                reference_entry2 = tkinter.Entry(reference_entry, width=5, textvariable=reference_value2)
                reference_entry2.pack(side='left')
                reference_value2.set(0)

                enhancement_name = tkinter.StringVar()
                channel_label = tkinter.Label(frame5, text='channel')
                channel_label.pack(side='left')
                if v.get()==0:
                    enhancement_name.set('gray')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, value='gray')
                else:
                    enhancement_name.set('R')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, 'R','G','B')
                channel_menu.pack(side = 'left')
                channel_menu.config(width=6)

            elif value == 'histogram equalization':
                reference_label = tkinter.Label(frame5, text='cutoff(%)')
                reference_label.pack(side='left')
                reference_value3 = tkinter.DoubleVar()
                reference_entry = tkinter.Entry(frame5, width=5, textvariable=reference_value3)
                reference_entry.pack(side='left')
                reference_value3.set(0)

                enhancement_name = tkinter.StringVar()
                channel_label = tkinter.Label(frame5, text='channel')
                channel_label.pack(side='left')
                if v.get()==0:
                    enhancement_name.set('gray')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, value='gray')
                else:
                    enhancement_name.set('R')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, 'R', 'G', 'B')
                channel_menu.pack(side = 'left')
                channel_menu.config(width=6)
            elif value == 'brightness change':
                reference_label = tkinter.Label(frame5, text='amplitude')
                reference_label.pack(side='left')
                reference_value4 = tkinter.DoubleVar()
                reference_entry = tkinter.Entry(frame5, width=5, textvariable=reference_value4)
                reference_entry.pack(side='left')
                reference_value4.set(1)

                enhancement_name = tkinter.StringVar()
                channel_label = tkinter.Label(frame5, text='channel')
                channel_label.pack(side='left')
                if v.get()==0:
                    enhancement_name.set('gray')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, value='gray')
                else:
                    enhancement_name.set('R')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, 'R', 'G', 'B')
                channel_menu.pack(side='left')
                channel_menu.config(width=6)
            elif value == 'contrast change':
                reference_label = tkinter.Label(frame5, text='amplitude')
                reference_label.pack(side='left')
                reference_value5 = tkinter.DoubleVar()
                reference_entry = tkinter.Entry(frame5, width=5, textvariable=reference_value5)
                reference_entry.pack(side='left')
                reference_value5.set(1)

                enhancement_name = tkinter.StringVar()
                channel_label = tkinter.Label(frame5, text='channel')
                channel_label.pack(side='left')
                if v.get()==0:
                    enhancement_name.set('gray')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, value='gray')
                else:
                    enhancement_name.set('R')
                    channel_menu = tkinter.OptionMenu(frame5, enhancement_name, 'R', 'G', 'B')
                channel_menu.pack(side='left')
                channel_menu.config(width=6)
            enhancement_button = tkinter.Button(frame5,text = 'apply',command = apply_enhancement)
            enhancement_button.pack(side='left')

        enhancement_menu = tkinter.OptionMenu(frame5, enhancement_value, *enhancement_list, command=change_function)
        enhancement_menu.pack(side='left')

        reference_value3 = tkinter.DoubleVar()
        reference_value4 = tkinter.DoubleVar()
        reference_value5 = tkinter.DoubleVar()

        reference_label = tkinter.Label(frame5, text='k and b')
        reference_label.pack(side='left')
        reference_value1 = tkinter.DoubleVar()
        reference_entry = tkinter.Frame(frame5)
        reference_entry.pack(side='left')
        reference_entry1 = tkinter.Entry(reference_entry, width=5, textvariable=reference_value1)
        reference_entry1.pack(side='left')
        reference_value1.set(1.0)

        reference_value2 = tkinter.DoubleVar()
        reference_entry2 = tkinter.Entry(reference_entry, width=5, textvariable=reference_value2)
        reference_entry2.pack(side='left')
        reference_value2.set(0)

        enhancement_name = tkinter.StringVar()
        channel_label = tkinter.Label(frame5, text='channel')
        channel_label.pack(side='left')
        enhancement_name.set('gray')
        channel_menu = tkinter.OptionMenu(frame5, enhancement_name, value='gray')
        channel_menu.pack(side = 'left')
        channel_menu.config(width=6)

        def apply_enhancement():
            nonlocal reference_value1
            nonlocal reference_value2
            nonlocal reference_value3
            nonlocal reference_value4
            nonlocal reference_value5
            nonlocal enhancement_name

            if enhancement_value.get()=='linear stretch':
                self.linear_stretch(reference_value1.get(),reference_value2.get(),enhancement_name.get())
            elif enhancement_value.get()=='histogram equalization':
                self.histogram_equalization(reference_value3.get(),enhancement_name.get())
            elif enhancement_value.get()=='brightness change':
                self.brightness_change(reference_value4.get(),enhancement_name.get())
            elif enhancement_value.get()=='contrast change':
                self.contrast_change(reference_value5.get(),enhancement_name.get())
        enhancement_button = tkinter.Button(frame5,text = 'apply',command = apply_enhancement)
        enhancement_button.pack(side = 'left')

        root.mainloop()

        f.close()


    def linear_stretch(self,k,b,channel):
        self.previous_img = self.im
        if channel=='gray':
            self.im = self.im.point(lambda x:k*x+b)
        else:
            R_image,G_image,B_image = self.im.split()
            if channel == 'R':
                R_image = R_image.point(lambda x:k*x+b)
            elif channel == 'G':
                G_image = G_image.point(lambda x:k*x+b)
            elif channel == 'B':
                B_image = B_image.point(lambda x:k*x+b)
            self.im = Image.merge('RGB', (R_image, G_image, B_image))
        self.show_image()

    def histogram_equalization(self,cutoff,channel):
        self.previous_img = self.im
        if channel == 'gray':
            self.im = ImageOps.autocontrast(self.im,cutoff)
        else:
            R_image,G_image,B_image = self.im.split()
            if channel == 'R':
                R_image = ImageOps.autocontrast(R_image,cutoff)
            elif channel == 'G':
                G_image = ImageOps.autocontrast(G_image,cutoff)
            elif channel == 'B':
                B_image = ImageOps.autocontrast(B_image,cutoff)
            self.im = Image.merge('RGB',(R_image,G_image,B_image))
        self.show_image()

    def brightness_change(self,percentage,channel):
        self.previous_img = self.im
        if channel == 'gray':
            enh = ImageEnhance.Brightness(self.im)
            self.im = enh.enhance(percentage)
        else:
            R_image, G_image, B_image = self.im.split()
            if channel == 'R':
                enh = ImageEnhance.Brightness(R_image)
                R_image = enh.enhance(percentage)
            elif channel == 'G':
                enh = ImageEnhance.Brightness(G_image)
                G_image = enh.enhance(percentage)
            elif channel == 'B':
                enh = ImageEnhance.Brightness(B_image)
                B_image = enh.enhance(percentage)
            self.im = Image.merge('RGB', (R_image, G_image, B_image))
        self.show_image()

    def contrast_change(self,percentage,channel):
        self.previous_img = self.im
        if channel == 'gray':
            enh = ImageEnhance.Contrast(self.im)
            self.im = enh.enhance(percentage)
        else:
            R_image, G_image, B_image = self.im.split()
            if channel == 'R':
                enh = ImageEnhance.Contrast(R_image)
                R_image = enh.enhance(percentage)
            elif channel == 'G':
                enh = ImageEnhance.Contrast(G_image)
                G_image = enh.enhance(percentage)
            elif channel == 'B':
                enh = ImageEnhance.Contrast(B_image)
                B_image = enh.enhance(percentage)
            self.im = Image.merge('RGB',(R_image,G_image,B_image))
        self.show_image()



test_image = Image_display()
test_image.GUI()
#test_image.show_image()
#test_image.preperation_for_display(mode = 'gray',channel = 2)
#test_image.show_image()

#test_image.linear_stretch(2,5)
#test_image.brightness_change(0.5)
#test_image.contrast_change(2)
#test_image.histogram_equalization(20)
