import pygame
import serial
import time
import numpy as np
# Initialize Pygame and Pygame mixer
pygame.init()
pygame.mixer.init()

# Screen dimensions
SCREEN_WIDTH = 1200
SCREEN_HEIGHT = 600


drum_names = ['high_hat', 'crash', 'rim', 'tomtom', "kick"]

# Load drum sounds
drum_sounds = [
    pygame.mixer.Sound(f'assets/{drum_names[0]}.wav'),
    pygame.mixer.Sound(f'assets/{drum_names[1]}.wav'),
    pygame.mixer.Sound(f'assets/{drum_names[2]}.wav'),
    pygame.mixer.Sound(f'assets/{drum_names[3]}.wav'),
    pygame.mixer.Sound(f'assets/{drum_names[4]}.wav')
]

# Load drum images
drum_images = [
    pygame.image.load(f'assets/{drum_names[0]}.jpeg'),
    pygame.image.load(f'assets/{drum_names[1]}.jpeg'),
    pygame.image.load(f'assets/{drum_names[2]}.jpeg'),
    pygame.image.load(f'assets/{drum_names[3]}.jpeg'),
    pygame.image.load(f'assets/{drum_names[4]}.jpeg')
]


drum_scales = np.array([1.0, 1.0, 1.0, 1.0, 1.0])

# Initialize screen
# screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
screen = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
pygame.display.set_caption('Virtual Drumsticks')

# Define colors
WHITE = (255, 255, 255)
BLACK = (26, 26, 26)

# Define drum positions and radii
drums = [
    (200, 400, 150),
    (500, 200, 150),
    (950, 200, 150),
    (1250, 400, 150),
    (725, 600, 150)
]

# scale the drum images
for i in range(len(drums)):
    drum_images[i] = pygame.transform.scale(drum_images[i], (2*drums[i][2], 2*drums[i][2]))


# Serial port configuration (modify as necessary)
SERIAL_PORT_right = '/dev/tty.usbmodem56478001'
SERIAL_PORT_left = '/dev/tty.usbmodem68460701'
SERIAL_PORT_bottom = "/dev/tty.usbmodem32728401"
BAUD_RATE = 9600

# Initialize serial connection
ser_right = serial.Serial(SERIAL_PORT_right, BAUD_RATE)
ser_left = serial.Serial(SERIAL_PORT_left, BAUD_RATE)
ser_bottom = serial.Serial(SERIAL_PORT_bottom, BAUD_RATE)


# Threshold angle
THRESHOLD_Y_ANGLE = -40
THRESHOLD_X_ANGLE_RIGHT = [360, 180] 
THRESHOLD_X_ANGLE_LEFT = [360, 180]
BOTTOM_THRESHOLD_Y_ANGLE = -20

# Function to draw drums
# def draw_drums():
#     for (x, y, radius) in drums:
#         pygame.draw.circle(screen, BLACK, (x, y), radius)

def play_sound(angles):
    print(angles)
    if angles[1] > THRESHOLD_Y_ANGLE:
        if 360 > angles[3] > THRESHOLD_X_ANGLE_LEFT[0] or 0 < angles[3] < THRESHOLD_X_ANGLE_LEFT[1]:
            if is_playing[0] == False:
                drum_sounds[0].play()
                drum_scales[0] = 1.2
            else:
                drum_scales[0] = 1.0
        else:
            if is_playing[0] == False:
                drum_sounds[1].play()
                drum_scales[1] = 1.2
            else:
                drum_scales[1] = 1.0
        is_playing[0] = True
    else:
        is_playing[0] = False
        
    if angles[0] > THRESHOLD_Y_ANGLE:
        if 360 > angles[2] > THRESHOLD_X_ANGLE_RIGHT[0] or 0 < angles[2] < THRESHOLD_X_ANGLE_RIGHT[1]:
            if is_playing[1] == False:
                drum_sounds[2].play()
                drum_scales[2] = 1.2
            else:
                drum_scales[2] = 1.0
        else:
            if is_playing[1] == False:
                drum_sounds[3].play()
                drum_scales[3] = 1.2
            else:
                drum_scales[3] = 1.0
        is_playing[1] = True
    else:
        is_playing[1] = False

    if angles[4] < BOTTOM_THRESHOLD_Y_ANGLE:
        if is_playing[2] == False:
            drum_sounds[4].play()
            drum_scales[4] = 1.2
            is_playing[2] = True
        else:
            drum_scales[4] = 1.0
    else:
        is_playing[2] = False

# Main loop
running = True
is_playing = [False, False, False]
init_time = time.time()
while running:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
    try:  
        # Clear the screen
        screen.fill(BLACK)
        
        # Draw the drums
        # draw_drums()
        
        # angles_right = [-1000, -1000]
        # angles_left = [-1000, -1000]
        # data_bottom = -1000

        # Read data from serial port
        if ser_right.in_waiting > 0:
            data_right = ser_right.readline().decode('utf-8').strip()
            angles_right = data_right.split(',')

        if ser_left.in_waiting > 0:
            data_left = ser_left.readline().decode('utf-8').strip()
            angles_left = data_left.split(',')
        
        if ser_bottom.in_waiting > 0:
            data_bottom = ser_bottom.readline().decode('utf-8').strip()
        
        
        angles = [ angles_right[0], angles_left[0], angles_right[1], angles_left[1], data_bottom]
        # angles = [ angles_right[0], -1000, angles_right[1], -1000, data_bottom]
        # angles = [ -1000, -1000, -1000, -1000, data_bottom]
        # angles = [-1000, angles_left[0], -1000, angles_left[1]]
            
        # Assuming we receive two angles, one for each drumstick
        if len(angles) != 5:
            continue
 
        play_sound([int(angle) for angle in angles])

        # Update the display
        if time.time() - init_time > 0.01 or any(drum_scales!=1.0):
            # Draw the drums
            for i, ((x, y, r), scale) in enumerate(zip(drums, drum_scales)):
                # drum_image = pygame.transform.scale(drum_images[i], (2*r*scale, 2*r*scale))
                # screen.blit(drum_image, (x - r, y - r))
                if drum_scales[i] != 1.0:
                    screen.blit(drum_images[i], (x - r, y - r-10))
                else:
                    screen.blit(drum_images[i], (x - r, y - r))
            pygame.display.update()
            init_time = time.time()
    except Exception as e:
        print(e)
        continue
    # Delay to limit frame rate
    # time.sleep(0.01)

# Quit Pygame
pygame.quit()
# ser_right.close()
# ser_left.close()
