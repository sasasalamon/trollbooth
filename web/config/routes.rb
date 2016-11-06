Rails.application.routes.draw do
  root 'trollbooth#index'

  get 'trollbooth/index'
  post 'trollbooth/open'
end
