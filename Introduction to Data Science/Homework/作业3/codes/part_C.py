import pandas as pd
from scipy.stats import pearsonr
import seaborn as sns
import matplotlib.pyplot as plt

file_path = "dataset/ai_job_trends_dataset_cleaned.csv"
df = pd.read_csv(file_path)

# 计算皮尔逊相关系数和 p 值
corr_coef, p_value = pearsonr(df['Median Salary (USD)'], df['Automation Risk (%)'])
print(f"皮尔逊相关系数: {corr_coef:.4f}")
print(f"p 值: {p_value:.4e}")

# 可视化散点图
sns.set_theme(style="whitegrid")
plt.figure(figsize=(8, 6))
sns.regplot(
    data=df,
    x='Median Salary (USD)',
    y='Automation Risk (%)',
    scatter_kws={'alpha': 0.6},
    line_kws={'color': 'red'}
)
plt.title('Median Salary vs Automation Risk', fontsize=14)
plt.xlabel('Median Salary (USD)', fontsize=12)
plt.ylabel('Automation Risk (%)', fontsize=12)
plt.tight_layout()
plt.savefig('imgs/median_salary_vs_automation_risk.png')
plt.show()

# 可视化箱线图和堆叠直方图

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
# 分组
q25 = df['Median Salary (USD)'].quantile(0.25)
q50 = df['Median Salary (USD)'].quantile(0.50)
q75 = df['Median Salary (USD)'].quantile(0.75)
def assign_salary_group(salary):
    if salary <= q25:
        return '低薪资'
    elif salary <= q50:
        return '中低薪资'
    elif salary <= q75:
        return '中高薪资'
    else:
        return '高薪资'
df['Salary Group'] = df['Median Salary (USD)'].apply(assign_salary_group)
salary_order = ['低薪资', '中低薪资', '中高薪资', '高薪资']
df['Salary Group'] = pd.Categorical(df['Salary Group'], categories=salary_order, ordered=True)
# 绘制箱线图
plt.figure(figsize=(10, 6))
sns.boxplot(data=df, x='Salary Group', y='Automation Risk (%)', order=salary_order)
plt.title('各薪资组的自动化风险分布')
plt.xlabel('薪资组')
plt.ylabel('自动化风险 (%)')
plt.savefig('imgs/automation_risk_distribution.png', dpi=300, bbox_inches='tight')
plt.show()
# 绘制堆叠柱状图
fig, axes = plt.subplots(1, 2, figsize=(16, 6))
ai_impact_ct = pd.crosstab(df['Salary Group'], df['AI Impact Level'], normalize='index') * 100
ai_impact_ct.plot(kind='bar', stacked=True, ax=axes[0], colormap='Set2')
axes[0].set_title('各薪资组中 AI 影响级别的比例 (%)')
axes[0].set_xlabel('薪资组')
axes[0].set_ylabel('比例 (%)')
axes[0].legend(title='AI Impact Level', bbox_to_anchor=(1.02, 1), loc='upper left')
job_status_ct = pd.crosstab(df['Salary Group'], df['Job Status'], normalize='index') * 100
job_status_ct.plot(kind='bar', stacked=True, ax=axes[1], colormap='tab10')
axes[1].set_title('各薪资组中工作增减状态的比例 (%)')
axes[1].set_xlabel('薪资组')
axes[1].set_ylabel('比例 (%)')
axes[1].legend(title='Job Status', bbox_to_anchor=(1.02, 1), loc='upper left')
plt.tight_layout(rect=(0, 0, 1, 0.95))  # 留出顶部标题空间（如果需要）
plt.savefig('imgs/impact_and_status_proportions.png', dpi=300, bbox_inches='tight')
plt.show()