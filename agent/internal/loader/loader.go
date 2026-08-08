/*
******************************************************************************

	PGDBATOOLS AGENT

	START  : 2026-08-06
	AUTHOR : Marco Túlio Castro

*******************************************************************************
*/
package loader

import (
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"gopkg.in/yaml.v3"
)

type QueryDefinition struct {
	Name          string `yaml:"name"`
	Description   string `yaml:"description"`
	LatestVersion int    `yaml:"latest_version"`
	FileName      string `yaml:"file_name"`
	Group         string `yaml:"-"`
	GroupLevel    int    `yaml:"-"`
	Path          string `yaml:"-"`
}

type Metrics struct {
	Queries []QueryDefinition `yaml:"queries"`
}

func allDirs() {
	root := "./queries"

	rootClean := filepath.Clean(root)

	err := filepath.WalkDir(rootClean, func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		relPath, err := filepath.Rel(rootClean, path)
		if err != nil {
			return err
		}

		nivel := 0
		if relPath != "." {
			partes := strings.Split(relPath, string(filepath.Separator))
			nivel = len(partes)
		}

		fmt.Printf("[Nível %d] %s\n", nivel, path)
		return nil
	})

	if err != nil {
		fmt.Printf("Erro: %v\n", err)
	}
}

func Load() (*Metrics, error) {
	root := "queries" // Diretório inicial da busca (ex: "." para a pasta atual)

	metrics := &Metrics{}

	err := filepath.WalkDir(root, func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		// Verifica se é um ficheiro e se o nome é "definition.yaml"
		if !d.IsDir() && d.Name() == "definition.yaml" {
			//fmt.Println("Encontrado:", path)
			parts := strings.Split(path, string(filepath.Separator))
			group := parts[len(parts)-2]

			data, err := os.ReadFile(path)
			if err != nil {
				return err
			}
			err = yaml.Unmarshal(data, metrics)
			if err != nil {
				return err
			}

			for index := range metrics.Queries {
				metrics.Queries[index].Group = group
				metrics.Queries[index].GroupLevel = len(parts)
				metrics.Queries[index].Path = path
			}
		}

		return nil
	})

	if err != nil {
		fmt.Fprintf(os.Stderr, "Error reading directory: %v\n", err)
	}
	return metrics, nil
}
