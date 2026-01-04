import cv2
import numpy as np
import time

def main():
    print("Hello")
    cam: cv2.VideoCapture = cv2.VideoCapture("/dev/video2")

    cam.set(cv2.CAP_PROP_FRAME_WIDTH,  640)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 360)

    FPS: int = 15
    
    while (True):

        ret, frame = cam.read()
        if not ret:
            continue
        
        dimensions = frame.shape[:2]
        
        #frame = cv2.resize(frame, (10, int(dimensions[0]*aspect)))

        left_half = cv2.mean(frame[:, :frame.shape[1] // 3])[:3]
        right_half = cv2.mean(frame[:, -frame.shape[1] // 3 :])[:3]

        #print(f"left: {left_half}, right: {right_half}")
        
        new_img_width = 20
        new_img_half = 10
        img5 = np.zeros((new_img_width, new_img_width, 3), dtype=np.uint8)

        # Left half of the 5×5
        img5[:, :new_img_half] = left_half  # columns 0–1

        # Right half of the 5×5
        img5[:, new_img_half:] = right_half  # columns 2–4

        # Enlarge so you can see it
        big = cv2.resize(img5, (300, 300), interpolation=cv2.INTER_NEAREST)

        cv2.imshow("Left/Right Mean Colors", big)

        WIDTH: int = 200
        aspect = WIDTH / dimensions[1]
        frame = frame = cv2.resize(frame, (WIDTH, int(dimensions[0]*aspect)))
        cv2.imshow("frame", frame)
        
        #cv2.imshow("Hello", frame)
        
        time.sleep(1 / FPS)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cam.release()
    cv2.destroyAllWindows()
    

if __name__ == "__main__":
    main()