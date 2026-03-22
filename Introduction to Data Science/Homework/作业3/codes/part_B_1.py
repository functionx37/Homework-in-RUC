import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# 加载数据
file_path = "dataset/ai_job_trends_dataset_cleaned.csv"
df = pd.read_csv(file_path)

# 不同学历的薪资水平和自动化风险
edu_order = [
    "High School",
    "Associate Degree",
    "Bachelor’s Degree",
    "Master’s Degree",
    "PhD",
]
df["Required Education"] = pd.Categorical(
    df["Required Education"], categories=edu_order, ordered=True
)
edu_summary = (
    df.groupby("Required Education", observed=True)
    .agg(
        Avg_Salary=("Median Salary (USD)", "mean"),
        Median_Salary=("Median Salary (USD)", "median"),
        Avg_Automation_Risk=("Automation Risk (%)", "mean"),
        Median_Automation_Risk=("Automation Risk (%)", "median"),
    )
    .reset_index()
)
print(edu_summary.to_string(index=False))

# 可视化（箱线图）
plt.figure(figsize=(10, 6))
sns.boxplot(data=df, x="Required Education", y="Median Salary (USD)", order=edu_order)
plt.title("Salary Distribution by Education Level")
plt.tight_layout()
plt.savefig("imgs/salary_by_education.png")
plt.show()
# 可视化（条形图）
plt.figure(figsize=(10, 6))
sns.barplot(data=edu_summary, x='Required Education', y='Avg_Automation_Risk', order=edu_order)
plt.title('Average Automation Risk by Education Level')
plt.ylabel('Average Automation Risk (%)')
plt.ylim(36, 46) 
plt.tight_layout()
plt.savefig("imgs/automation_risk_by_education.png")
plt.show()

# 不同经验的薪资水平和自动化风险
exp_bins = [0, 5, 10, 15, 20]
exp_labels = ['0-5 Years', '6-10 Years', '11-15 Years', '16-20 Years']
df['Experience Group'] = pd.cut(df['Experience Required (Years)'], bins=exp_bins, labels=exp_labels, right=False)
exp_summary = (
    df.groupby("Experience Group",observed=True)
    .agg(
        Avg_Salary=("Median Salary (USD)", "mean"),
        Median_Salary=("Median Salary (USD)", "median"),
        Avg_Automation_Risk=("Automation Risk (%)", "mean"),
        Median_Automation_Risk=("Automation Risk (%)", "median"),
    )
    .reset_index()
)
print(exp_summary.to_string(index=False))

# 可视化（箱线图）
plt.figure(figsize=(10, 6))
sns.boxplot(data=df, x="Experience Group", y="Median Salary (USD)", order=exp_labels)
plt.title("Salary Distribution by Experience Level")
plt.tight_layout()
plt.savefig("imgs/salary_by_experience.png")
plt.show()
# 可视化（条形图）
plt.figure(figsize=(10, 6))
sns.barplot(data=exp_summary, x='Experience Group', y='Avg_Automation_Risk', order=exp_labels)
plt.title('Average Automation Risk by Experience Level')
plt.ylabel('Average Automation Risk (%)')
plt.ylim(35, 50)
plt.tight_layout()
plt.savefig("imgs/automation_risk_by_experience.png")
plt.show()

# 散点图 
edu_order = ['High School', 'Associate Degree', 'Bachelor’s Degree', 'Master’s Degree', 'PhD']
df['Required Education'] = pd.Categorical(df['Required Education'], categories=edu_order, ordered=True)
risk_median = df['Automation Risk (%)'].median()
salary_median = df['Median Salary (USD)'].median()
df_sampled = df.sample(n=1500, random_state=42)
sizes = (df_sampled['Experience Required (Years)'] + 1) * 8 
palette = sns.color_palette("Set2", len(edu_order))
plt.figure(figsize=(12, 10))
sns.scatterplot(
    data=df_sampled,
    x='Automation Risk (%)',
    y='Median Salary (USD)',
    hue='Required Education',
    size=df_sampled['Experience Required (Years)'],
    sizes=(20, 200),  
    palette=palette,
    alpha=0.7,
    edgecolor='k',
    linewidth=0.3
)
plt.axvline(x=risk_median, color='gray', linestyle='--', linewidth=1)
plt.axhline(y=salary_median, color='gray', linestyle='--', linewidth=1)
plt.title('Quadrant Analysis: Automation Risk vs Salary (2024–2030 AI Job Trends)', fontsize=14)
plt.xlabel('Automation Risk (%)')
plt.ylabel('Median Salary (USD)')
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.savefig('imgs/automation_risk_vs_salary.png', dpi=300, bbox_inches='tight')
plt.show()

# 热力图
edu_order = ['High School', 'Associate Degree', 'Bachelor’s Degree', 'Master’s Degree', 'PhD']
bins = [0, 5, 9, 13, 17, 21] 
labels = ['0-4', '5-8', '9-12', '13-16', '17-20']
df['Experience_Bin'] = pd.cut(
    df['Experience Required (Years)'],
    bins=bins,
    labels=labels,
    include_lowest=True,   
    right=False            
)
grouped = df.groupby(['Required Education', 'Experience_Bin'], observed=True).agg({
    'Automation Risk (%)': 'mean',
    'Median Salary (USD)': 'mean'
}).reset_index()
grouped['Required Education'] = pd.Categorical(
    grouped['Required Education'],
    categories=edu_order,
    ordered=True
)
grouped['Experience_Bin'] = pd.Categorical(
    grouped['Experience_Bin'],
    categories=labels,
    ordered=True
)
risk_pivot = grouped.pivot_table(
    index='Required Education',
    columns='Experience_Bin',
    values='Automation Risk (%)',
    observed=True
)
salary_pivot = grouped.pivot_table(
    index='Required Education',
    columns='Experience_Bin',
    values='Median Salary (USD)',
    observed=True
)
risk_pivot = risk_pivot.reindex(edu_order[::-1])     
salary_pivot = salary_pivot.reindex(edu_order[::-1]) 
fig, axes = plt.subplots(1, 2, figsize=(16, 7))
sns.heatmap(
    risk_pivot,
    annot=True,
    fmt=".1f",
    cmap="RdYlGn_r",  
    cbar_kws={'label': 'Avg Automation Risk (%)'},
    ax=axes[0],
    linewidths=0.5
)
axes[0].set_title('Average Automation Risk by Education & Experience', fontsize=14)
axes[0].set_xlabel('Experience (Years)')
axes[0].set_ylabel('Required Education')
sns.heatmap(
    salary_pivot,
    annot=True,
    fmt=".0f",
    cmap="YlGnBu",
    cbar_kws={'label': 'Avg Median Salary (USD)'},
    ax=axes[1],
    linewidths=0.5
)
axes[1].set_title('Average Salary by Education & Experience', fontsize=14)
axes[1].set_xlabel('Experience (Years)')
axes[1].set_ylabel('Required Education')
plt.tight_layout()
plt.savefig('imgs/education_experience_heatmaps.png', dpi=300, bbox_inches='tight')
plt.show()