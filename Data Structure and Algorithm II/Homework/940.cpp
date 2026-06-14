#include <iostream>
#include <vector>
#include <algorithm>
#include <set> // Used for unique critical troop values initially, then converted to vector

// It's good practice to use long long for scores if they can exceed 2*10^9,
// but max score here is around n*k*n_max_val = 100*100*100 = 10^6, so int is fine.

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    // std::cout.tie(NULL); // Can sometimes provide a minor speedup for output-heavy tasks, not critical here.

    int k_opponents, n_fortresses, m_soldiers;
    std::cin >> k_opponents >> n_fortresses >> m_soldiers;

    // Corrected nested template syntax: std::vector<std::vector<int> >
    std::vector<std::vector<int>> opponent_forces(k_opponents, std::vector<int>(n_fortresses));
    for (int i = 0; i < k_opponents; ++i)
    {
        for (int j = 0; j < n_fortresses; ++j)
        {
            std::cin >> opponent_forces[i][j];
        }
    }

    // fortress_points[f_idx][t_sent] = total score obtained for fortress f_idx (1-based) if we send t_sent troops.
    // Fortress f_idx has intrinsic value f_idx.
    // Dimensions: (n_fortresses + 1) x (m_soldiers + 1)
    std::vector<std::vector<int>> fortress_points(n_fortresses + 1, std::vector<int>(m_soldiers + 1, 0));

    for (int f_idx = 1; f_idx <= n_fortresses; ++f_idx)
    { // Fortress index (1-based)
        std::vector<int> thresholds;
        thresholds.reserve(k_opponents); // Minor optimization: reserve space
        for (int opp_idx = 0; opp_idx < k_opponents; ++opp_idx)
        {
            thresholds.push_back(2 * opponent_forces[opp_idx][f_idx - 1] + 1);
        }
        std::sort(thresholds.begin(), thresholds.end());

        int current_opponents_beaten = 0;
        int threshold_ptr = 0;
        // fortress_points[f_idx][0] is already 0 by initialization.
        for (int t_sent = 1; t_sent <= m_soldiers; ++t_sent)
        {
            while (threshold_ptr < k_opponents && t_sent >= thresholds[threshold_ptr])
            {
                current_opponents_beaten++;
                threshold_ptr++;
            }
            fortress_points[f_idx][t_sent] = current_opponents_beaten * f_idx;
        }
    }

    // critical_xs_for_fortress[f_idx] stores sorted unique troop counts to consider for fortress f_idx
    // Dimensions: (n_fortresses + 1) x (at most k_opponents + 1)
    std::vector<std::vector<int>> critical_xs_for_fortress(n_fortresses + 1);
    for (int f_idx = 1; f_idx <= n_fortresses; ++f_idx)
    {
        std::set<int> unique_critical_xs;
        unique_critical_xs.insert(0);
        for (int opp_idx = 0; opp_idx < k_opponents; ++opp_idx)
        {
            int needed = 2 * opponent_forces[opp_idx][f_idx - 1] + 1;
            if (needed <= m_soldiers)
            {
                unique_critical_xs.insert(needed);
            }
        }
        critical_xs_for_fortress[f_idx].reserve(unique_critical_xs.size()); // Minor optimization
        critical_xs_for_fortress[f_idx].assign(unique_critical_xs.begin(), unique_critical_xs.end());
    }

    // DP state:
    // dp[j] = max score considering fortresses processed so far, using exactly j soldiers.
    // next_dp[j] = max score considering fortresses up to current one, using exactly j soldiers.
    std::vector<int> dp(m_soldiers + 1, 0);
    std::vector<int> next_dp(m_soldiers + 1, 0); // Using a temporary array for calculations of the current stage

    // Iterate through each fortress (item group)
    for (int fortress_idx = 1; fortress_idx <= n_fortresses; ++fortress_idx)
    {
        // Initialize next_dp for the current fortress processing stage.
        // Scores are non-negative, so 0 is a valid initial minimum.
        std::fill(next_dp.begin(), next_dp.end(), 0);

        const std::vector<int> &current_critical_allocations = critical_xs_for_fortress[fortress_idx];

        // For each critical troop allocation 'x_val' for the current_fortress
        for (size_t k_loop_idx = 0; k_loop_idx < current_critical_allocations.size(); ++k_loop_idx)
        {
            int x_val = current_critical_allocations[k_loop_idx];    // Troops allocated to current_fortress
            int points_for_x = fortress_points[fortress_idx][x_val]; // Score from current_fortress for x_val troops

            // For each possible total number of soldiers 'j_total' up to m_soldiers
            for (int j_total = x_val; j_total <= m_soldiers; ++j_total)
            {
                // If we use x_val troops for current_fortress,
                // then j_total - x_val troops were used for previous fortresses.
                // The score from previous fortresses is dp[j_total - x_val].
                next_dp[j_total] = std::max(next_dp[j_total], dp[j_total - x_val] + points_for_x);
            }
        }
        // After considering all choices for the current fortress, update dp to next_dp for the next iteration.
        dp = next_dp;
    }

    int max_overall_score = 0;
    // The final dp array contains max scores using up to n_fortresses for various total soldier counts.
    // We need the maximum among these.
    for (int j = 0; j <= m_soldiers; ++j)
    {
        max_overall_score = std::max(max_overall_score, dp[j]);
    }

    std::cout << max_overall_score << std::endl;

    return 0;
}
