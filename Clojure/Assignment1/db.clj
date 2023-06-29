(ns db
  (:require [clojure.string :as str])
  (:require [clojure.pprint :refer [pprint]])
  (:require [clojure.java.io]))

(def letterGrades {
"A+" 4.3, "A" 4, "A-" 3.7,
"B+" 3.3, "B" 3, "B-" 2.7,
"C+" 2.3, "C" 2, "C-" 1.7,
"D+" 1.3, "D" 1, "D-" 0.7,
"F" 0 }
)

(defn- collToMap
  [coll]
  (reduce (fn [acc [k & v]] (assoc acc k v)) {} coll)
  )

(defn loadData 
  [filename]
  (if (.exists (clojure.java.io/file filename))
    (mapv #(str/split % #"\|") (str/split-lines (slurp filename)))
    (do
      (println (str "ERROR: File <" filename "> could not be found.\nNOTE: Files in this assignment are hardcoded. You should define these files in the same directory as the app.clj file: [studs.txt, grades.txt, courses.txt]"))
      (System/exit 0)) 
    ) 
  )

(defn displayStudentRecords
  [studDB gradeDB courseDB]
  (def mutableList (atom (list)))
  (let [id (do
             (print "Enter an id: ")
             (flush)
             (read-line))
        student (into [] (take 2 (nth (filter #(= (nth % 0) id) studDB) 0)))
        grades (filter #(= (nth % 0) id) gradeDB)]
    (pprint student)
    (doseq [grade grades]
      (let [courseID (nth grade 1)
            course (nth (filter #(= (nth % 0) courseID) courseDB) 0)]
        (swap! mutableList conj (into [] (concat (conj (map #(nth course %) [4]) (apply str (interpose " " (map #(nth course %) [1 2])))) (map #(nth grade %) [2 3]))))
        )
      ) 
    (dorun (map pprint (reverse (sort-by #(nth % 0) @mutableList))))
    )
  )

(defn- weightedAVG 
  [grades weights]
  
  (/ (reduce + (map * grades weights)) (reduce + weights))
  )

(defn calGPA
  [gradeDB courseDB]
  (let [studentID (do
                    (print "Enter an id: ")
                    (flush)
                    (read-line))
        grades (map #(get letterGrades %) (map #(nth % 3) (filter #(= (nth % 0) studentID) gradeDB))) 
        courseMap (collToMap courseDB) 
        courseWeights (map #(read-string (nth % 2)) (map #(get courseMap %) (map #(nth % 1) (filter #(= (nth % 0) studentID) gradeDB))))]
    (pprint (str "GPA=" (weightedAVG grades courseWeights)))
    )
  )

(defn- avg 
  [coll]
  (/ (reduce + coll) (count coll))
  )

(defn courseAVG
  [gradeDB courseDB] 
  (def semesterGrades (atom (hash-map)))
  (doseq [grade gradeDB]
    (let [semester (nth grade 2)
          courseID (nth grade 1)
          course (nth (filter #(= (nth % 0) courseID) courseDB) 0)
          key (keyword (apply str (interpose " " (conj (conj `() semester) (apply str (interpose " " (map #(nth course %) [1 2]))))))) 
          score (get letterGrades (nth grade 3))
          ]
      (swap! semesterGrades update key #(if % (conj % score) (into [] [score])))
      )
  )
  (doseq [key (keys @semesterGrades)]
    (let [value (get @semesterGrades key)
          courseAverage (conj (into [] (conj `() (name key))) (avg value))]
      (pprint courseAverage)
      ) 
    )
)
