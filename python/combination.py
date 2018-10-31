"""
combination.py
Artur Rodrigues Rocha Neto
2018

Script gerador de vetores de característica otimizados através da combinação
de diferentes configurações (cortes, fatiamentos, momentos, etc.)
"""

from dataset import *
from features import *
from classification import *

def combination(dataset):
	basepath = "../results/"
	slices = ["frontal/", "radial/", "sagittal/", "transversal/", "whole/"]
	moments = ["neutral-hu1980.dat",
		       "neutral-hututu.dat",
		       "neutral-legendre.dat",
		       "neutral-chebyshev.dat",
		       "neutral-zernike.dat"]

	moments = [dataset + "/" + m for m in moments]
	products = list(itertools.product(slices, moments))
	configs = [basepath + p[0] + p[1] for p in products]
	combos = list(itertools.combinations(configs, 2))
	settings = [(c[0].split("/")[2], os.path.basename(c[0]), c[1].split("/")[2], os.path.basename(c[1])) for c in combos]

	cols = ["setting", "classifier", "rate"]
	df = pd.DataFrame(columns=cols)
	
	for setting, combo in zip(settings, combos):
		classifier, rate = max_rate(rank1_concat(combo))
		rate = round(rate*100, 2)
		setting = str(setting)
		result = "{:<85}{:<15}{:<7}".format(setting, classifier, rate)
		row = {"setting" : setting, "classifier" : classifier, "rate" : rate}
		df = df.append(row, ignore_index=True)
		print(result)

	df.to_csv("../results/combination/{}.csv".format(dataset[:-1]), index=False)

if __name__ == "__main__":
	datasets = ["bosphorus",
			    "bosphorus-outlier",
			    "bosphorus-outlier-densit200",
			    "bosphorus-outlier-densit200-crop60",
			    "bosphorus-outlier-densit200-crop70",
			    "bosphorus-outlier-densit200-crop80",
			    "bosphorus-outlier-densit200-crop60-icp",
	            "bosphorus-outlier-densit200-crop70-icp",
	            "bosphorus-outlier-densit200-crop80-icp",
			    "bosphorus-outlier-densit225",
			    "bosphorus-outlier-densit225-crop60",
			    "bosphorus-outlier-densit225-crop70",
			    "bosphorus-outlier-densit225-crop80",
	            "bosphorus-outlier-densit225-crop60-icp",
	            "bosphorus-outlier-densit225-crop70-icp",
	            "bosphorus-outlier-densit225-crop80-icp"]
	
	for dataset in datasets:
		combination(dataset)

