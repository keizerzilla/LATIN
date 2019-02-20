from dataset import *
from features import *
from classification import *

faces = ["neutral", "nonneutral"]

cuts = {"w" : "whole",
        "f" : "frontal",
        "s" : "sagittal",
        "t" : "transversal",
        "r" : "radial",
        "u" : "upper",
        "l" : "lower"}

replace_dict = {"bosphorus" : "bs",
                "outlier" : "out",
                "densit" : "d",
                "crop" : "c"}

scenarios= ["bosphorus",
            "bosphorus-outlier",
            "bosphorus-outlier-densit200",
            "bosphorus-outlier-densit225",
            "bosphorus-outlier-densit200-crop60",
			"bosphorus-outlier-densit200-crop70",
			"bosphorus-outlier-densit200-crop80",
			"bosphorus-outlier-densit225-crop60",
			"bosphorus-outlier-densit225-crop70",
			"bosphorus-outlier-densit225-crop80",
            "bosphorus-outlier-densit200-crop60-icp",
			"bosphorus-outlier-densit200-crop70-icp",
			"bosphorus-outlier-densit200-crop80-icp",
			"bosphorus-outlier-densit225-crop60-icp",
			"bosphorus-outlier-densit225-crop70-icp",
			"bosphorus-outlier-densit225-crop80-icp"]

moments = ["spheric",
           "hututu",
           "hu1980",
           "husiq",
           "zernike",
           "legendre",
           "chebyshev"]

def go_extraction(rdir):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			for data in datasets:
				scenario = data.split("/")[2]
				folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
				os.makedirs(folder, exist_ok=True)
				for moment in moments:
					out = folder
					out = out + "{}-{}.dat".format(face, moment)
					moment_extraction_batch(moment, cut, data, out)

def go_classification(rdir):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			for data in datasets:
				scenario = data.split("/")[2]
				folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
				os.makedirs(folder, exist_ok=True)
				
				print("[{}] - [{}]".format(cut_folder, folder))
				
				dataset = scenario
				for old, new in replace_dict.items():
					dataset = dataset.replace(old, new)
				
				for moment in moments:
					neutral = folder + "neutral-{}.dat".format(moment)
					
					ans = None
					if face == "neutral":
						ans = rank1_neutral(neutral)
					else:
						nonneutral = folder + "nonneutral-{}.dat".format(moment)
						ans = rank1_nonneutral(neutral, nonneutral)
					
					classifier, rate = max_rate(ans)
					rate = round(rate*100, 2)
					
					print("{:<11}{:<15}{:<7}".format(moment, classifier, rate))

def grid_search():
	print("ok")

def go_combination():
	for dataset in scenarios:
		combination_rank1_neutral(dataset, moments)
		combination_rank1_nonneutral(dataset, moments)
		#combination_rank1_neutral(dataset, moments, 3)
		#combination_rank1_nonneutral(dataset, moments, 3)

if __name__ == "__main__":
	go_extraction("results")
	#go_classification("results")
	#go_combination()
	
