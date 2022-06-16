多串口应用示例工程：

UART1  --> 打印调试串口，支持printf， 管脚：A9:Tx  A10:Rx  波特率：115200
UART2  --> 用户串口  管脚：A2:Tx  A3:Rx        波特率：115200
UART3  --> 用户串口  管脚：B10:Tx  B11:Rx     波特率： 9600

示例demo提供了几个测试项，根据enum_UART_TEST_t枚举选择特定的功能：
TEST_LOOP：串口查询接收和发送测试

TEST_DMA：测试串口DMA发送方式

TEST_IT：测试定长数据串口中断接收和发送

TEST_UART_ABORT：测试串口终止传输

TEST_UART2：测试串口2的中断接收和发送

TEST_UART3：测试串口3的中断接收和发送