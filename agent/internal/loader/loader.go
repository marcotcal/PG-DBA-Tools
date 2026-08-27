/*
******************************************************************************

	PGDBATOOLS AGENT

	START  : 2026-08-06
	AUTHOR : Marco Túlio Castro

*******************************************************************************
*/
package loader

import (
	"errors"
	"fmt"
	"io/fs"
	"log"
	"os"
	"path/filepath"
	"pgdbagent/internal/config"
	"sort"
	"strings"

	"gopkg.in/yaml.v3"
)

type QueryDefinition struct {
	Name          string `yaml:"name"`
	Description   string `yaml:"description"`
	LatestVersion int    `yaml:"latest_version"`
	FileName      string `yaml:"file_name"`
	Query         string `yaml:"-"`
}

type CommandDefinition struct {
	Name            string `yaml:"name"`
	Description     string `yaml:"description"`
	FileName        string `yaml:"file_name"`
	OperatingSystem string `yaml:"-"`
	Command         string `yaml:"-"`
}

type Metrics struct {
	Queries           []QueryDefinition   `yaml:"queries"`
	Commands          []CommandDefinition `yaml:"commands"`
	Group             string              `yaml:"-"`
	GroupLevel        int                 `yaml:"-"`
	Path              string              `yaml:"-"`
	VersionsInPath    []string            `yaml:"-"`
	PostgreSQLVersion string              `yaml:"-"`
}

func (qd *QueryDefinition) loadQueryFromFile(path string, pgVersion string, versions []string, name string) string {

	for _, version := range versions {

		if version > pgVersion {
			continue
		}

		queryPath := path + "/" + version + "/" + name

		_, err := os.Stat(queryPath)
		if err == nil {
			// file exists
			query, err := os.ReadFile(queryPath)
			if err != nil {
				log.Fatal(err)
			}
			return string(query)
		} else if errors.Is(err, os.ErrNotExist) {
			// file does not exists
			continue
		} else {
			// another error
			log.Fatal(err)
		}

	}
	return ""

}

func (cmd *CommandDefinition) loadCmd(path string, operationSystem string, name string) {

}

func Load(cfg *config.Config) (*[]Metrics, error) {

	root := "queries" // Diretório inicial da busca (ex: "." para a pasta atual)
	list := []Metrics{}

	for _, cluster := range cfg.Instances {

		err := filepath.WalkDir(root, func(path string, d fs.DirEntry, err error) error {
			if err != nil {
				return err
			}

			// Verifica se é um ficheiro e se o nome é "definition.yaml"
			if !d.IsDir() && d.Name() == "definition.yaml" {

				var metrics Metrics

				parts := strings.Split(path, string(filepath.Separator))
				group := filepath.Base(filepath.Dir(path))
				dirPath := filepath.Dir(path)

				data, err := os.ReadFile(path)
				if err != nil {
					return err
				}

				versionDirs, err := os.ReadDir(dirPath)
				if err != nil {
					log.Fatal(err)
				}

				sort.Slice(versionDirs, func(i, j int) bool {
					return versionDirs[i].Name() > versionDirs[j].Name()
				})

				metrics.VersionsInPath = make([]string, len(versionDirs)-1)

				err = yaml.Unmarshal(data, &metrics)
				if err != nil {
					return err
				}

				for i, version := range versionDirs {
					if err != nil {
						log.Fatal(err)
					}
					if version.Name() == "definition.yaml" {
						continue
					}
					metrics.VersionsInPath[i-1] = version.Name()
				}

				metrics.Group = group
				metrics.GroupLevel = len(parts)
				metrics.Path = path
				metrics.PostgreSQLVersion = cluster.MajorVersion

				for index := range metrics.Queries {

					query := metrics.Queries[index].loadQueryFromFile(dirPath, cluster.MajorVersion,
						metrics.VersionsInPath, metrics.Queries[index].FileName)

					metrics.Queries[index].Query = query

				}

				list = append(list, metrics)
			}

			return nil
		})

		if err != nil {
			fmt.Fprintf(os.Stderr, "Error reading directory: %v\n", err)
		}
	}

	return &list, nil
}
