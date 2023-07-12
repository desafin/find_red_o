import cv2
import numpy as np

def nothing(x):
    pass

# 이미지 파일 경로
image_path = 'find_google_area.png'

# 이미지 파일 읽기
frame = cv2.imread(image_path)

bgrLower = np.array([25,0,100])  # 추출할 색의 하한(BGR)
bgrUpper = np.array([90,85,200])  # 추출할 색의 상한(BGR)
img_mask = cv2.inRange(frame, bgrLower, bgrUpper)  # BGR로 부터 마스크를 작성
result = cv2.bitwise_and(frame, frame, mask=img_mask)  # 원본 이미지와 마스크를 합성

cv2.imshow("red",result)

gray = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
cv2.imshow("gray",gray)


threshold = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY)
cv2.imshow("threshold",threshold[1])

# Find contours
contours, _ = cv2.findContours(threshold[1], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)


# Set minimum contour area threshold
min_contour_area = 100  # Adjust this value according to your needs

# Filter out small contours
filtered_contours = []
for i in contours:
    contour_area = cv2.contourArea(i)
    if contour_area > min_contour_area:
        filtered_contours.append(i)


# Draw contours on the original image
contour_image = np.copy(frame)
cv2.drawContours(contour_image, filtered_contours, -1, (0, 0, 0), 2)
cv2.imshow("filtered_contours",contour_image)

# Display the image with contours
cv2.imshow("Contours", contour_image)


# Rotate and draw rotated bounding boxes on the original image
contour_image_with_rotated_boxes = np.copy(frame)
for contour in filtered_contours:
    # Find the rotated bounding box
    rect = cv2.minAreaRect(contour)
    box = cv2.boxPoints(rect)
    box = np.int0(box)

    # Rotate the bounding box by a specified angle (in degrees)
    angle = 10  # Adjust the angle as desired
    M = cv2.getRotationMatrix2D(rect[0], angle, 1)
    rotated_box = cv2.transform(np.array([box]), M)[0]
    cv2.putText(contour_image_with_rotated_boxes, "angle is" + str(angle), (0, 20),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)


    # Draw the rotated bounding box on the image
    cv2.drawContours(contour_image_with_rotated_boxes, [rotated_box], 0, (0, 0, 0), 2)

# Display the image with contours and rotated bounding boxes
cv2.imshow("Contours with Rotated Boxes", contour_image_with_rotated_boxes)


# Draw bounding boxes on the original image
contour_image_with_boxes = np.copy(frame)
for contour in filtered_contours:
    x, y, w, h = cv2.boundingRect(contour)
    cv2.rectangle(contour_image_with_boxes, (x, y), (x+w, y+h), (0, 0, 0), 2)
    center_x = x + (w // 2)
    center_y = y + (h // 2)
    print("컨투어 박스 중심좌표: ({}, {})".format(center_x, center_y))

# Calculate average brightness within the bounding box
for contour in filtered_contours:
    x, y, w, h = cv2.boundingRect(contour)
    bounding_box = gray[y:y + h, x:x + w]  # Extract the region within the bounding box
    average_brightness = np.mean(bounding_box)

# Display the image with contours and bounding boxes
cv2.imshow("Contours with Bounding Boxes", contour_image_with_boxes)
cv2.putText(contour_image_with_rotated_boxes, "center is "+str((center_x, center_y)), (center_x, center_y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)



cv2.putText(contour_image_with_rotated_boxes, "Area is " + str(w*h), (0, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0, 0, 0), 2)
cv2.putText(contour_image_with_rotated_boxes, "ratio is " + str(w/h), (0, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0, 0, 0), 2)
cv2.putText(contour_image_with_rotated_boxes, "width is " + str(w), (0, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0, 0, 0), 2)
cv2.putText(contour_image_with_rotated_boxes, "height is " + str(h), (0, 100), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0, 0, 0), 2)
cv2.putText(contour_image_with_rotated_boxes, "brightness is " + str(average_brightness), (0, 120), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0, 0, 0), 2)



cv2.imshow("putText with Rotated Boxes", contour_image_with_rotated_boxes)
key = cv2.waitKey(0)

