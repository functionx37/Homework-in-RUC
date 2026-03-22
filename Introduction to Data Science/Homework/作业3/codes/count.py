import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = "dataset/ai_job_trends_dataset_cleaned.csv"
df = pd.read_csv(file_path)

# 对数值型字段进行描述性统计
numeric_fields = [
    "Median Salary (USD)",
    "Automation Risk (%)",
    "Experience Required (Years)",
    "Remote Work Ratio (%)",
    "Gender Diversity (%)",
    "Job Openings (2024)",
    "Projected Openings (2030)"
]

desc_stats = df[numeric_fields].describe()


summary_table = desc_stats.T
summary_table = summary_table[['mean', '50%', 'std', 'min', '25%', '75%', 'max']]
summary_table.rename(columns={
    'mean': '均值',
    '50%': '中位数',
    'std': '标准差',
    'min': '最小值',
    '25%': '下四分位数 (Q1)',
    '75%': '上四分位数 (Q3)',
    'max': '最大值'
}, inplace=True)
summary_table = summary_table.round(2)

print("数值型字段描述性统计：")
print(summary_table)

# 可视化

# 箱线图
sns.set_style("whitegrid")
plt.rcParams['figure.figsize'] = (28, 6) 
fig, axes = plt.subplots(1, 7, figsize=(28, 6))
for idx, field in enumerate(numeric_fields):
    ax = axes[idx]
    sns.boxplot(y=df[field], ax=ax, width=0.5)
    ax.set_title(field, fontsize=12, pad=10)
    ax.set_ylabel('') 
    ax.tick_params(axis='x', rotation=0)
plt.subplots_adjust(wspace=0.2, left=0.05, right=0.98, bottom=0.15, top=0.85)
plt.savefig("imgs/ai_job_trends_boxplots.png", dpi=300, bbox_inches='tight')
plt.show()
# 频率分布直方图
fig, axes = plt.subplots(1, len(numeric_fields), figsize=(28, 6), sharey=True)
fig.suptitle('AI Job Trends: Frequency Distribution Histograms', fontsize=16, y=0.98)
for idx, field in enumerate(numeric_fields):
    sns.histplot(
        data=df[[field]],
        x=field,
        ax=axes[idx],
        bins=20,
        kde=False,
        color='skyblue',
        edgecolor='black',
        linewidth=0.5
    )
    axes[idx].set_title(field, fontsize=12, pad=10)
    axes[idx].set_xlabel('')
    axes[idx].set_ylabel('')
    axes[idx].tick_params(axis='x', labelsize=9)
plt.subplots_adjust(wspace=0.2, left=0.05, right=0.98, bottom=0.15, top=0.85)
plt.savefig("imgs/ai_job_trends_histograms.png", dpi=300, bbox_inches='tight')
plt.show()

# 对分类型字段进行频次统计

# 各行业的频次和占比
industry_counts = df['Industry'].value_counts()
total = industry_counts.sum()
industry_summary = pd.DataFrame({
    'Industry': industry_counts.index,
    'Count': industry_counts.values,
    'Percentage': industry_counts.div(total).mul(100)  
})
print("\n行业频次与占比：")
print(industry_summary.to_string(index=False))
# 各学历要求的频次与占比
edu_counts = df['Required Education'].value_counts(normalize=True).mul(100).reset_index()
edu_counts.columns = ['Education Level', 'Percentage']
edu_counts['Count'] = df['Required Education'].value_counts().values
print("\n教育水平频次与占比：")
print(edu_counts.to_string(index=False))
# 职位状态频次与占比
status_counts = df['Job Status'].value_counts(normalize=True).mul(100).reset_index()
status_counts.columns = ['Job Status', 'Percentage']
status_counts['Count'] = df['Job Status'].value_counts().values
print("\n职位状态频次与占比：")
print(status_counts.to_string(index=False))
# 工作国家频次与占比
location_counts = df['Location'].value_counts()
total = location_counts.sum()
location_summary = pd.DataFrame({
    'Location': location_counts.index,
    'Count': location_counts.values,
    'Percentage': location_counts.div(total).mul(100)  # 安全的除法方式
})
print("\n工作国家频次与占比：")
print(location_summary.to_string(index=False))

# 饼状图可视化

categorical_fields = [
    "Industry",
    "Required Education",
    "Job Status",
    "Location"
]
fig, axes = plt.subplots(2, 2, figsize=(16, 12))
axes = axes.flatten()
cmap = plt.colormaps['Pastel1']
colors = [cmap(i) for i in range(cmap.N)]
for idx, col in enumerate(categorical_fields):
    counts = df[col].value_counts()
    ax = axes[idx]
    ax.pie(
        counts.values,
        labels=counts.index,
        autopct='%1.1f%%',
        startangle=90,
        colors=colors[:len(counts)]
    )
    ax.set_title(col, fontsize=14, pad=12)
plt.tight_layout(pad=2.5)
plt.savefig("imgs/ai_job_trends_piecharts.png", dpi=300, bbox_inches='tight')
plt.show()