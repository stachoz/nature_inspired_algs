import pandas as pd
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
if os.path.exists(os.path.join(BASE_DIR, "results")):
    RESULTS_DIR = os.path.join(BASE_DIR, "results")
else:
    RESULTS_DIR = BASE_DIR

OUTPUT_FILE = os.path.join(BASE_DIR, "final_results.xlsx")

def generate_excel():
    files = [f for f in os.listdir(RESULTS_DIR) if f.endswith('.csv')]

    if not files:
        print(f"Brak plikow CSV w lokalizacji: {RESULTS_DIR}")
        return

    with pd.ExcelWriter(OUTPUT_FILE, engine='openpyxl') as writer:
        for file in sorted(files):
            file_path = os.path.join(RESULTS_DIR, file)
            df = pd.read_csv(file_path)

            sheet_name = file.replace(".csv", "")

            iterations = [20, 50, 100, 500]
            final_sheet_df = pd.DataFrame()

            for it in iterations:
                subset = df[df['iteration'] == it][['f1', 'f2']].reset_index(drop=True)

                subset.columns = [f'f1 (it_{it})', f'f2 (it_{it})']

                if final_sheet_df.empty:
                    final_sheet_df = subset
                else:
                    final_sheet_df = pd.concat([final_sheet_df, subset], axis=1)

            final_sheet_df.to_excel(writer, sheet_name=sheet_name, index=False)
            print(f"Dodano zakladke: {sheet_name}")

    print(f"\nPlik zapisany jako: {OUTPUT_FILE}")

if __name__ == "__main__":
    generate_excel()