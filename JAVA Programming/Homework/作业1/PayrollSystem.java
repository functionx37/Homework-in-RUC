import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * 员工抽象基类，定义了所有员工的共有属性和行为
 */
abstract class Staff {
    protected String name; // 员工姓名

    public Staff(String name) {
        this.name = name;
    }

    /**
     * 计算工资的抽象方法，由具体的子类去实现
     * @return 计算后的工资金额
     */
    public abstract double calculateSalary();
    
    /**
     * 获取员工类型的抽象方法
     * @return 员工类型字符串
     */
    public abstract String getType();

    public String getName() {
        return name;
    }
}

/**
 * 全职员工类，继承自Staff
 */
class FullTimeStaff extends Staff {
    private double dailySalary;         // 每天的基本工资
    private double overtimeHourlyRate;  // 超时工作每小时的收入
    private int daysWorked;             // 工作的天数
    private int overtimeHours;          // 超时工作的小时数

    public FullTimeStaff(String name, double dailySalary, double overtimeHourlyRate, int daysWorked, int overtimeHours) {
        super(name);
        this.dailySalary = dailySalary;
        this.overtimeHourlyRate = overtimeHourlyRate;
        this.daysWorked = daysWorked;
        this.overtimeHours = overtimeHours;
    }

    /**
     * 实现父类的计算工资方法
     * 全职员工工资 = 基本工资 * 工作天数 + 加班时薪 * 加班小时数
     */
    @Override
    public double calculateSalary() {
        return dailySalary * daysWorked + overtimeHourlyRate * overtimeHours;
    }

    @Override
    public String getType() {
        return "FullTime";
    }
}

/**
 * 兼职员工类，继承自Staff
 */
class PartTimeStaff extends Staff {
    private double hourlyRate; // 每小时工作的收入
    private int hoursWorked;   // 工作的小时数

    public PartTimeStaff(String name, double hourlyRate, int hoursWorked) {
        super(name);
        this.hourlyRate = hourlyRate;
        this.hoursWorked = hoursWorked;
    }

    /**
     * 实现父类的计算工资方法
     * 兼职员工工资 = 时薪 * 工作小时数
     */
    @Override
    public double calculateSalary() {
        return hourlyRate * hoursWorked;
    }

    @Override
    public String getType() {
        return "PartTime";
    }
}

/**
 * 工资系统主程序类，包含系统入口
 */
public class PayrollSystem {
    public static void main(String[] args) {
        // 1. 检查命令行参数数量是否正确 (需要输入文件、员工类型、输出文件)
        if (args.length != 3) {
            System.out.println("用法: java PayrollSystem <输入文件名> <员工类型(fulltime/parttime)> <输出文件名>");
            System.out.println("示例: java PayrollSystem fulltime.txt fulltime output.txt");
            return;
        }

        String inputFile = args[0];
        String employeeType = args[1];
        String outputFile = args[2];

        // 用于存储读取和解析到的员工对象列表
        List<Staff> staffList = new ArrayList<>();

        // 2. 读取数据文件并解析为员工对象
        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            String line = br.readLine();
            if (line == null) {
                System.out.println("输入文件为空。");
                return;
            }

            // 文件的第一行表示记录的数量
            int count = Integer.parseInt(line.trim());
            
            // 逐行读取每一条员工记录
            for (int i = 0; i < count; i++) {
                line = br.readLine();
                if (line == null) break;

                // 使用 "\\|" 转义管道符来分割字符串数据
                String[] parts = line.split("\\|");
                
                // 根据命令行传入的员工类型进行相应的解析操作
                if (employeeType.equalsIgnoreCase("fulltime")) {
                    // 确保数据格式正确：名字|每天基本工资|加班时薪|工作天数|加班小时数
                    if (parts.length == 5) {
                        String name = parts[0];
                        double dailySalary = Double.parseDouble(parts[1]);
                        double overtimeRate = Double.parseDouble(parts[2]);
                        int days = Integer.parseInt(parts[3]);
                        int overtimeHours = Integer.parseInt(parts[4]);
                        
                        // 创建全职员工对象并加入列表
                        staffList.add(new FullTimeStaff(name, dailySalary, overtimeRate, days, overtimeHours));
                    }
                } else if (employeeType.equalsIgnoreCase("parttime")) {
                    // 确保数据格式正确：名字|时薪|工作小时数
                    if (parts.length == 3) {
                        String name = parts[0];
                        double hourlyRate = Double.parseDouble(parts[1]);
                        int hours = Integer.parseInt(parts[2]);
                        
                        // 创建兼职员工对象并加入列表
                        staffList.add(new PartTimeStaff(name, hourlyRate, hours));
                    }
                } else {
                    System.out.println("未知的员工类型: " + employeeType);
                    return;
                }
            }
        } catch (IOException e) {
            System.out.println("读取文件时出错: " + e.getMessage());
            return;
        } catch (NumberFormatException e) {
            System.out.println("解析文件中的数字时出错: " + e.getMessage());
            return;
        }

        // 3. 将结果输出到屏幕并存储到输出文件中
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
            for (Staff staff : staffList) {
                // 格式化输出字符串，保留两位小数显示工资
                String resultLine = String.format("姓名: %s, 类型: %s, 工资: %.2f", 
                        staff.getName(), staff.getType(), staff.calculateSalary());
                
                // 打印到屏幕
                System.out.println(resultLine);
                
                // 写入到指定的输出文件
                bw.write(resultLine);
                bw.newLine();
            }
            System.out.println("-> 结果已成功写入到文件: " + outputFile);
        } catch (IOException e) {
            System.out.println("写入文件时出错: " + e.getMessage());
        }
    }
}