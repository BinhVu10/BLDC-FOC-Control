# FOC Control STM32H743

## Motivation & Goal

Project thực hiện điều khiển động cơ BLDC/PMSM bằng **Field-Oriented Control (FOC)** trên **STM32H743VIH6**.

Mục tiêu chính:
- Xây dựng vòng điều khiển dòng `Id/Iq`.
- Thực thi Clarke/Park và inverse transforms.
- Sinh điện áp pha bằng **SVPWM**.
- Đọc dòng pha qua ADC và đồng bộ với PWM.
- Đọc encoder để xác định góc điện `theta_e`.
- Calibrate current sensor và encoder offset.
- Sau khi vòng dòng hoạt động ổn định, mở rộng lên vòng tốc độ/vị trí (sẽ phát triển trong tương lai)

Phần công suất sử dụng **DRV8317H**, cấu hình **3x PWM**.

## Project Structure

```text
/Core               STM32CubeMX generated code và cấu hình ngoại vi mức thấp.
/platform           Hardware abstraction cho ADC, PWM, encoder, motor driver,...
/lib/foc            Thuật toán FOC cốt lõi: transforms, current loop interface, SVPWM.
/lib/controller     Thực thi các bộ điều khiển.
/app                Logic vận hành motor, state machine và phối hợp các module, các mode thực thi.
/config             Tham số motor, board và các tham số điều khiển.
```

Mục tiêu của cấu trúc trên là giữ phần thuật toán điều khiển càng độc lập với STM32 HAL và phần cứng cụ thể càng tốt nhằm mang tính thừa kế cao.

## High-Level Flow

```text
Init
  -> peripheral / driver / motor modules
  -> calibration
  -> start operation
  -> PWM timer triggers ADC sampling
  -> ADC conversion complete interrupt
       -> acquire current feedback
       -> acquire encoder angle
       -> run FOC current-control cycle
       -> calculate SVPWM
       -> update PWM duty
```

**FOC cycle = current-control-loop cycle** và được đồng bộ với PWM/ADC sampling.

Vòng `while (1)` không chạy FOC. Về sau nó sẽ liên tục theo dõi và phản ứng với các trạng thái bất thường như `nFAULT`, lỗi driver, lỗi hệ thống,... Phần fault handling này **chưa được phát triển ở giai đoạn hiện tại**.

## Design Direction

```text
Application -> FOC / Controllers -> Platform -> STM32 HAL
```

Đây chỉ là kiến trúc tổng quan hiện tại. Chi tiết module, API và execution flow sẽ tiếp tục được điều chỉnh khi project phát triển và các yêu cầu thực tế trở nên rõ ràng hơn.


Đây là 1 project nên tảng cốt lõi nhất của điều khiển động cơ, agent nên hiểu về cấu trúc project, ngữ cảnh, code để tiện cho việc phát triển lâu dài