/*******************************************************************************
   PGDBATOOLS AGENT

   START  : 2026-08-06
   AUTHOR : Marco Túlio Castro
********************************************************************************/

package config

import (
	"os"

	"gopkg.in/yaml.v3"
)

type Config struct {
	Agent     AgentConfig      `yaml:"agent"`
	Server    ServerConfig     `yaml:"server"`
	Instances []InstanceConfig `yaml:"instances"`
}

type AgentConfig struct {
	Name     string `yaml:"name"`
	Interval string `yaml:"interval"`
	LogLevel string `yaml:"log_level"`
}

type ServerConfig struct {
	URL    string `yaml:"url"`
	APIKey string `yaml:"api_key"`
}

type InstanceConfig struct {
	Name     string `yaml:"name"`
	Host     string `yaml:"host"`
	Port     int    `yaml:"port"`
	Database string `yaml:"database"`
	User     string `yaml:"user"`
	Password string `yaml:"password"`
}

func Load(filename string) (*Config, error) {

	data, err := os.ReadFile(filename)
	if err != nil {
		return nil, err
	}

	cfg := &Config{}

	err = yaml.Unmarshal(data, cfg)
	if err != nil {
		return nil, err
	}

	for index, db := range cfg.Instances {

		if db.Password == "" {
			pass := os.Getenv("PG_PASSWORD")
			if pass != "" {
				cfg.Instances[index].Password = pass
			}
		}

	}

	return cfg, nil
}

func Validate(cfg *Config) error {

	return nil
}
